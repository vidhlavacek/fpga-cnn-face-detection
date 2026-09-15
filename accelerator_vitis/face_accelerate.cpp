#include "cnn_layers.hpp"

void face_accelerate(
    const data_t *input,
    const weight_t *stem_weights,   const weight_t *stem_bias,
    const weight_t *block1_weights, const weight_t *block1_bias,
    const weight_t *block2_weights, const weight_t *block2_bias,
    const weight_t *head0_weights,  const weight_t *head0_bias,
    const weight_t *head1_weights,  const weight_t *head1_bias,
    data_t *scratch1, data_t *scratch2, data_t *scratch3, data_t *scratch4,
    data_t *output
)
{
    //gmem0: mape značajki
    #pragma HLS INTERFACE m_axi port=input     offset=slave bundle=gmem0 depth=3*320*320
    #pragma HLS INTERFACE m_axi port=scratch1  offset=slave bundle=gmem0 depth=16*160*160
    #pragma HLS INTERFACE m_axi port=scratch2  offset=slave bundle=gmem0 depth=24*80*80
    #pragma HLS INTERFACE m_axi port=scratch3  offset=slave bundle=gmem0 depth=48*40*40
    #pragma HLS INTERFACE m_axi port=scratch4  offset=slave bundle=gmem0 depth=48*40*40
    #pragma HLS INTERFACE m_axi port=output    offset=slave bundle=gmem0 depth=6*40*40

    //gmem1: tezine i biasi
    #pragma HLS INTERFACE m_axi port=stem_weights   offset=slave bundle=gmem1 depth=16*3*3*3
    #pragma HLS INTERFACE m_axi port=stem_bias      offset=slave bundle=gmem1 depth=16
    #pragma HLS INTERFACE m_axi port=block1_weights offset=slave bundle=gmem1 depth=24*16*3*3
    #pragma HLS INTERFACE m_axi port=block1_bias    offset=slave bundle=gmem1 depth=24
    #pragma HLS INTERFACE m_axi port=block2_weights offset=slave bundle=gmem1 depth=48*24*3*3
    #pragma HLS INTERFACE m_axi port=block2_bias    offset=slave bundle=gmem1 depth=48
    #pragma HLS INTERFACE m_axi port=head0_weights  offset=slave bundle=gmem1 depth=48*48*3*3
    #pragma HLS INTERFACE m_axi port=head0_bias     offset=slave bundle=gmem1 depth=48
    #pragma HLS INTERFACE m_axi port=head1_weights  offset=slave bundle=gmem1 depth=6*48*1*1
    #pragma HLS INTERFACE m_axi port=head1_bias     offset=slave bundle=gmem1 depth=6

    //s_axilite: registri za adrese
    #pragma HLS INTERFACE s_axilite port=input          bundle=control
    #pragma HLS INTERFACE s_axilite port=stem_weights   bundle=control
    #pragma HLS INTERFACE s_axilite port=stem_bias      bundle=control
    #pragma HLS INTERFACE s_axilite port=block1_weights bundle=control
    #pragma HLS INTERFACE s_axilite port=block1_bias    bundle=control
    #pragma HLS INTERFACE s_axilite port=block2_weights bundle=control
    #pragma HLS INTERFACE s_axilite port=block2_bias    bundle=control
    #pragma HLS INTERFACE s_axilite port=head0_weights  bundle=control
    #pragma HLS INTERFACE s_axilite port=head0_bias     bundle=control
    #pragma HLS INTERFACE s_axilite port=head1_weights  bundle=control
    #pragma HLS INTERFACE s_axilite port=head1_bias     bundle=control
    #pragma HLS INTERFACE s_axilite port=scratch1       bundle=control
    #pragma HLS INTERFACE s_axilite port=scratch2       bundle=control
    #pragma HLS INTERFACE s_axilite port=scratch3       bundle=control
    #pragma HLS INTERFACE s_axilite port=scratch4       bundle=control
    #pragma HLS INTERFACE s_axilite port=output         bundle=control
    #pragma HLS INTERFACE s_axilite port=return         bundle=control

    // stem: 3x320x320 -> 16x160x160, k3 s2 p1, ReLU
    conv2d_layer<3, 16, 320, 320, 3, 2, 1, true>(
        input, stem_weights, stem_bias, scratch1);

    // block1: 16x160x160 -> 24x80x80, k3 s2 p1, ReLU
    conv2d_layer<16, 24, 160, 160, 3, 2, 1, true>(
        scratch1, block1_weights, block1_bias, scratch2);

    // block2: 24x80x80 -> 48x40x40, k3 s2 p1, ReLU
    conv2d_layer<24, 48, 80, 80, 3, 2, 1, true>(
        scratch2, block2_weights, block2_bias, scratch3);

    // head.0: 48x40x40 -> 48x40x40, k3 s1 p1, ReLU
    conv2d_layer<48, 48, 40, 40, 3, 1, 1, true>(
        scratch3, head0_weights, head0_bias, scratch4);

    // head.1: 48x40x40 -> 6x40x40, k1 s1 p0, BEZ ReLU (finalni detekcijski izlaz)
    conv2d_layer<48, 6, 40, 40, 1, 1, 0, false>(
        scratch4, head1_weights, head1_bias, output);
}
