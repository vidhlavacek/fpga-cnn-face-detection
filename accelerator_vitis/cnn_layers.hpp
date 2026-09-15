#ifndef CNN_LAYERS_HPP
#define CNN_LAYERS_HPP

#include <ap_fixed.h>

typedef ap_fixed<16, 6>  data_t;
typedef ap_fixed<16, 6>  weight_t;
typedef ap_fixed<32, 16> accum_t;

void face_accelerate(
    const data_t *input,
    const weight_t *stem_weights,   const weight_t *stem_bias,
    const weight_t *block1_weights, const weight_t *block1_bias,
    const weight_t *block2_weights, const weight_t *block2_bias,
    const weight_t *head0_weights,  const weight_t *head0_bias,
    const weight_t *head1_weights,  const weight_t *head1_bias,
    data_t *scratch1, data_t *scratch2, data_t *scratch3, data_t *scratch4,
    data_t *output
);

template
<
    int IN_CHANNELS,
    int OUT_CHANNELS,
    int IN_H,
    int IN_W,
    int KERNEL,
    int STRIDE,
    int PAD,
    bool RELU
>void conv2d_layer
(
    const data_t *input,
    const weight_t *weights,
    const weight_t *bias,
    data_t *output
) {
    const int OUT_H = (IN_H + 2 * PAD - KERNEL) / STRIDE + 1;
    const int OUT_W = (IN_W + 2 * PAD - KERNEL) / STRIDE + 1;

    const int TILE_H = 4;
    const int TILE_W = 4;

    const int TILE_IN_H = (TILE_H - 1) * STRIDE + KERNEL;
    const int TILE_IN_W = (TILE_W - 1) * STRIDE + KERNEL;

    for (int tile_y = 0; tile_y < OUT_H; tile_y += TILE_H)
    {
        for (int tile_x = 0; tile_x < OUT_W; tile_x += TILE_W)
        {
            data_t input_tile[IN_CHANNELS][TILE_IN_H][TILE_IN_W];
            #pragma HLS ARRAY_PARTITION variable=input_tile cyclic factor=KERNEL dim=2
            #pragma HLS ARRAY_PARTITION variable=input_tile cyclic factor=KERNEL dim=3

            LOAD_TILE:
            for (int ic = 0; ic < IN_CHANNELS; ic++) {
                for (int ty = 0; ty < TILE_IN_H; ty++) {
                    for (int tx = 0; tx < TILE_IN_W; tx++) {
                        #pragma HLS PIPELINE II=1
                        const int iy = tile_y * STRIDE - PAD + ty;
                        const int ix = tile_x * STRIDE - PAD + tx;

                        data_t v = 0;
                        if (iy >= 0 && iy < IN_H && ix >= 0 && ix < IN_W) {
                            v = input[ic * IN_H * IN_W + iy * IN_W + ix];
                        }
                        input_tile[ic][ty][tx] = v;
                    }
                }
            }

            for (int oc = 0; oc < OUT_CHANNELS; oc++)
            {
                accum_t out_tile[TILE_H][TILE_W];
                #pragma HLS ARRAY_PARTITION variable=out_tile complete dim=0

                INIT_TILE:
                for (int ti = 0; ti < TILE_H; ti++) {
                    for (int tj = 0; tj < TILE_W; tj++) {
                        #pragma HLS PIPELINE II=1
                        out_tile[ti][tj] = (accum_t)bias[oc];
                    }
                }

                for (int ic = 0; ic < IN_CHANNELS; ic++)
                {
                    weight_t kernel_buf[KERNEL][KERNEL];
                    #pragma HLS ARRAY_PARTITION variable=kernel_buf complete dim=0

                    LOAD_KERNEL:
                    for (int k = 0; k < KERNEL; k++) {
                        for (int l = 0; l < KERNEL; l++) {
                            #pragma HLS PIPELINE II=1
                            const int weight_idx =
                                ((oc * IN_CHANNELS + ic) * KERNEL + k)
                                * KERNEL + l;
                            kernel_buf[k][l] = weights[weight_idx];
                        }
                    }

                    COMPUTE_TILE:
                    for (int ti = 0; ti < TILE_H; ti++) {
                        for (int tj = 0; tj < TILE_W; tj++) {
                            if constexpr (KERNEL == 1) {
                                #pragma HLS PIPELINE II=2
                            } else {
                                #pragma HLS PIPELINE II=1
                            }

                            const int oy = tile_y + ti;
                            const int ox = tile_x + tj;

                            if (oy < OUT_H && ox < OUT_W)
                            {
                                accum_t channel_sum = 0;
                                for (int k = 0; k < KERNEL; k++) {
                                    for (int l = 0; l < KERNEL; l++) {
                                        #pragma HLS UNROLL factor=3

                                        const int local_y = ti * STRIDE + k;
                                        const int local_x = tj * STRIDE + l;

                                        channel_sum +=
                                            kernel_buf[k][l] *
                                            input_tile[ic][local_y][local_x];
                                    }
                                }

                                out_tile[ti][tj] += channel_sum;
                            }
                        }
                    }
                }

                WRITE_TILE:
                for (int ti = 0; ti < TILE_H; ti++) {
                    for (int tj = 0; tj < TILE_W; tj++) {
                        #pragma HLS PIPELINE II=1

                        const int oy = tile_y + ti;
                        const int ox = tile_x + tj;

                        if (oy < OUT_H && ox < OUT_W)
                        {
                            accum_t value = out_tile[ti][tj];

                            if (RELU && value < 0) {
                                value = 0;
                            }

                            const int output_idx =
                                oc * OUT_H * OUT_W + oy * OUT_W + ox;

                            output[output_idx] = (data_t)value;
                        }
                    }
                }
            }
        }
    }
}

#endif