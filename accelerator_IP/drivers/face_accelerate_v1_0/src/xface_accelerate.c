// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xface_accelerate.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XFace_accelerate_CfgInitialize(XFace_accelerate *InstancePtr, XFace_accelerate_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XFace_accelerate_Start(XFace_accelerate *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_AP_CTRL) & 0x80;
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XFace_accelerate_IsDone(XFace_accelerate *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XFace_accelerate_IsIdle(XFace_accelerate *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XFace_accelerate_IsReady(XFace_accelerate *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XFace_accelerate_EnableAutoRestart(XFace_accelerate *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XFace_accelerate_DisableAutoRestart(XFace_accelerate *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_AP_CTRL, 0);
}

void XFace_accelerate_Set_input_r(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_INPUT_R_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_INPUT_R_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_input_r(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_INPUT_R_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_INPUT_R_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_stem_weights(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_STEM_WEIGHTS_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_STEM_WEIGHTS_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_stem_weights(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_STEM_WEIGHTS_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_STEM_WEIGHTS_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_stem_bias(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_STEM_BIAS_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_STEM_BIAS_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_stem_bias(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_STEM_BIAS_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_STEM_BIAS_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_block1_weights(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK1_WEIGHTS_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK1_WEIGHTS_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_block1_weights(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK1_WEIGHTS_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK1_WEIGHTS_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_block1_bias(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK1_BIAS_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK1_BIAS_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_block1_bias(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK1_BIAS_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK1_BIAS_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_block2_weights(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK2_WEIGHTS_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK2_WEIGHTS_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_block2_weights(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK2_WEIGHTS_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK2_WEIGHTS_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_block2_bias(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK2_BIAS_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK2_BIAS_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_block2_bias(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK2_BIAS_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_BLOCK2_BIAS_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_head0_weights(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD0_WEIGHTS_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD0_WEIGHTS_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_head0_weights(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD0_WEIGHTS_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD0_WEIGHTS_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_head0_bias(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD0_BIAS_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD0_BIAS_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_head0_bias(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD0_BIAS_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD0_BIAS_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_head1_weights(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD1_WEIGHTS_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD1_WEIGHTS_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_head1_weights(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD1_WEIGHTS_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD1_WEIGHTS_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_head1_bias(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD1_BIAS_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD1_BIAS_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_head1_bias(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD1_BIAS_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_HEAD1_BIAS_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_scratch1(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH1_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH1_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_scratch1(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH1_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH1_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_scratch2(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH2_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH2_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_scratch2(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH2_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH2_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_scratch3(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH3_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH3_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_scratch3(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH3_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH3_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_scratch4(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH4_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH4_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_scratch4(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH4_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_SCRATCH4_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_Set_output_r(XFace_accelerate *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_OUTPUT_R_DATA, (u32)(Data));
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_OUTPUT_R_DATA + 4, (u32)(Data >> 32));
}

u64 XFace_accelerate_Get_output_r(XFace_accelerate *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_OUTPUT_R_DATA);
    Data += (u64)XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_OUTPUT_R_DATA + 4) << 32;
    return Data;
}

void XFace_accelerate_InterruptGlobalEnable(XFace_accelerate *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_GIE, 1);
}

void XFace_accelerate_InterruptGlobalDisable(XFace_accelerate *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_GIE, 0);
}

void XFace_accelerate_InterruptEnable(XFace_accelerate *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_IER);
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_IER, Register | Mask);
}

void XFace_accelerate_InterruptDisable(XFace_accelerate *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_IER);
    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_IER, Register & (~Mask));
}

void XFace_accelerate_InterruptClear(XFace_accelerate *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XFace_accelerate_WriteReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_ISR, Mask);
}

u32 XFace_accelerate_InterruptGetEnabled(XFace_accelerate *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_IER);
}

u32 XFace_accelerate_InterruptGetStatus(XFace_accelerate *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XFace_accelerate_ReadReg(InstancePtr->Control_BaseAddress, XFACE_ACCELERATE_CONTROL_ADDR_ISR);
}

