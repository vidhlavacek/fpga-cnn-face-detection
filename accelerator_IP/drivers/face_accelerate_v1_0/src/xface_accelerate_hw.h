// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
// control
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 7  - auto_restart (Read/Write)
//        bit 9  - interrupt (Read)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0 - enable ap_done interrupt (Read/Write)
//        bit 1 - enable ap_ready interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0 - ap_done (Read/TOW)
//        bit 1 - ap_ready (Read/TOW)
//        others - reserved
// 0x10 : Data signal of input_r
//        bit 31~0 - input_r[31:0] (Read/Write)
// 0x14 : Data signal of input_r
//        bit 31~0 - input_r[63:32] (Read/Write)
// 0x18 : reserved
// 0x1c : Data signal of stem_weights
//        bit 31~0 - stem_weights[31:0] (Read/Write)
// 0x20 : Data signal of stem_weights
//        bit 31~0 - stem_weights[63:32] (Read/Write)
// 0x24 : reserved
// 0x28 : Data signal of stem_bias
//        bit 31~0 - stem_bias[31:0] (Read/Write)
// 0x2c : Data signal of stem_bias
//        bit 31~0 - stem_bias[63:32] (Read/Write)
// 0x30 : reserved
// 0x34 : Data signal of block1_weights
//        bit 31~0 - block1_weights[31:0] (Read/Write)
// 0x38 : Data signal of block1_weights
//        bit 31~0 - block1_weights[63:32] (Read/Write)
// 0x3c : reserved
// 0x40 : Data signal of block1_bias
//        bit 31~0 - block1_bias[31:0] (Read/Write)
// 0x44 : Data signal of block1_bias
//        bit 31~0 - block1_bias[63:32] (Read/Write)
// 0x48 : reserved
// 0x4c : Data signal of block2_weights
//        bit 31~0 - block2_weights[31:0] (Read/Write)
// 0x50 : Data signal of block2_weights
//        bit 31~0 - block2_weights[63:32] (Read/Write)
// 0x54 : reserved
// 0x58 : Data signal of block2_bias
//        bit 31~0 - block2_bias[31:0] (Read/Write)
// 0x5c : Data signal of block2_bias
//        bit 31~0 - block2_bias[63:32] (Read/Write)
// 0x60 : reserved
// 0x64 : Data signal of head0_weights
//        bit 31~0 - head0_weights[31:0] (Read/Write)
// 0x68 : Data signal of head0_weights
//        bit 31~0 - head0_weights[63:32] (Read/Write)
// 0x6c : reserved
// 0x70 : Data signal of head0_bias
//        bit 31~0 - head0_bias[31:0] (Read/Write)
// 0x74 : Data signal of head0_bias
//        bit 31~0 - head0_bias[63:32] (Read/Write)
// 0x78 : reserved
// 0x7c : Data signal of head1_weights
//        bit 31~0 - head1_weights[31:0] (Read/Write)
// 0x80 : Data signal of head1_weights
//        bit 31~0 - head1_weights[63:32] (Read/Write)
// 0x84 : reserved
// 0x88 : Data signal of head1_bias
//        bit 31~0 - head1_bias[31:0] (Read/Write)
// 0x8c : Data signal of head1_bias
//        bit 31~0 - head1_bias[63:32] (Read/Write)
// 0x90 : reserved
// 0x94 : Data signal of scratch1
//        bit 31~0 - scratch1[31:0] (Read/Write)
// 0x98 : Data signal of scratch1
//        bit 31~0 - scratch1[63:32] (Read/Write)
// 0x9c : reserved
// 0xa0 : Data signal of scratch2
//        bit 31~0 - scratch2[31:0] (Read/Write)
// 0xa4 : Data signal of scratch2
//        bit 31~0 - scratch2[63:32] (Read/Write)
// 0xa8 : reserved
// 0xac : Data signal of scratch3
//        bit 31~0 - scratch3[31:0] (Read/Write)
// 0xb0 : Data signal of scratch3
//        bit 31~0 - scratch3[63:32] (Read/Write)
// 0xb4 : reserved
// 0xb8 : Data signal of scratch4
//        bit 31~0 - scratch4[31:0] (Read/Write)
// 0xbc : Data signal of scratch4
//        bit 31~0 - scratch4[63:32] (Read/Write)
// 0xc0 : reserved
// 0xc4 : Data signal of output_r
//        bit 31~0 - output_r[31:0] (Read/Write)
// 0xc8 : Data signal of output_r
//        bit 31~0 - output_r[63:32] (Read/Write)
// 0xcc : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XFACE_ACCELERATE_CONTROL_ADDR_AP_CTRL             0x00
#define XFACE_ACCELERATE_CONTROL_ADDR_GIE                 0x04
#define XFACE_ACCELERATE_CONTROL_ADDR_IER                 0x08
#define XFACE_ACCELERATE_CONTROL_ADDR_ISR                 0x0c
#define XFACE_ACCELERATE_CONTROL_ADDR_INPUT_R_DATA        0x10
#define XFACE_ACCELERATE_CONTROL_BITS_INPUT_R_DATA        64
#define XFACE_ACCELERATE_CONTROL_ADDR_STEM_WEIGHTS_DATA   0x1c
#define XFACE_ACCELERATE_CONTROL_BITS_STEM_WEIGHTS_DATA   64
#define XFACE_ACCELERATE_CONTROL_ADDR_STEM_BIAS_DATA      0x28
#define XFACE_ACCELERATE_CONTROL_BITS_STEM_BIAS_DATA      64
#define XFACE_ACCELERATE_CONTROL_ADDR_BLOCK1_WEIGHTS_DATA 0x34
#define XFACE_ACCELERATE_CONTROL_BITS_BLOCK1_WEIGHTS_DATA 64
#define XFACE_ACCELERATE_CONTROL_ADDR_BLOCK1_BIAS_DATA    0x40
#define XFACE_ACCELERATE_CONTROL_BITS_BLOCK1_BIAS_DATA    64
#define XFACE_ACCELERATE_CONTROL_ADDR_BLOCK2_WEIGHTS_DATA 0x4c
#define XFACE_ACCELERATE_CONTROL_BITS_BLOCK2_WEIGHTS_DATA 64
#define XFACE_ACCELERATE_CONTROL_ADDR_BLOCK2_BIAS_DATA    0x58
#define XFACE_ACCELERATE_CONTROL_BITS_BLOCK2_BIAS_DATA    64
#define XFACE_ACCELERATE_CONTROL_ADDR_HEAD0_WEIGHTS_DATA  0x64
#define XFACE_ACCELERATE_CONTROL_BITS_HEAD0_WEIGHTS_DATA  64
#define XFACE_ACCELERATE_CONTROL_ADDR_HEAD0_BIAS_DATA     0x70
#define XFACE_ACCELERATE_CONTROL_BITS_HEAD0_BIAS_DATA     64
#define XFACE_ACCELERATE_CONTROL_ADDR_HEAD1_WEIGHTS_DATA  0x7c
#define XFACE_ACCELERATE_CONTROL_BITS_HEAD1_WEIGHTS_DATA  64
#define XFACE_ACCELERATE_CONTROL_ADDR_HEAD1_BIAS_DATA     0x88
#define XFACE_ACCELERATE_CONTROL_BITS_HEAD1_BIAS_DATA     64
#define XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH1_DATA       0x94
#define XFACE_ACCELERATE_CONTROL_BITS_SCRATCH1_DATA       64
#define XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH2_DATA       0xa0
#define XFACE_ACCELERATE_CONTROL_BITS_SCRATCH2_DATA       64
#define XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH3_DATA       0xac
#define XFACE_ACCELERATE_CONTROL_BITS_SCRATCH3_DATA       64
#define XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH4_DATA       0xb8
#define XFACE_ACCELERATE_CONTROL_BITS_SCRATCH4_DATA       64
#define XFACE_ACCELERATE_CONTROL_ADDR_OUTPUT_R_DATA       0xc4
#define XFACE_ACCELERATE_CONTROL_BITS_OUTPUT_R_DATA       64

