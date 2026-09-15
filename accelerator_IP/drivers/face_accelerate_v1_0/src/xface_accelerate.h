// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XFACE_ACCELERATE_H
#define XFACE_ACCELERATE_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xface_accelerate_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
#ifdef SDT
    char *Name;
#else
    u16 DeviceId;
#endif
    u64 Control_BaseAddress;
} XFace_accelerate_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XFace_accelerate;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XFace_accelerate_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XFace_accelerate_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XFace_accelerate_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XFace_accelerate_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
#ifdef SDT
int XFace_accelerate_Initialize(XFace_accelerate *InstancePtr, UINTPTR BaseAddress);
XFace_accelerate_Config* XFace_accelerate_LookupConfig(UINTPTR BaseAddress);
#else
int XFace_accelerate_Initialize(XFace_accelerate *InstancePtr, u16 DeviceId);
XFace_accelerate_Config* XFace_accelerate_LookupConfig(u16 DeviceId);
#endif
int XFace_accelerate_CfgInitialize(XFace_accelerate *InstancePtr, XFace_accelerate_Config *ConfigPtr);
#else
int XFace_accelerate_Initialize(XFace_accelerate *InstancePtr, const char* InstanceName);
int XFace_accelerate_Release(XFace_accelerate *InstancePtr);
#endif

void XFace_accelerate_Start(XFace_accelerate *InstancePtr);
u32 XFace_accelerate_IsDone(XFace_accelerate *InstancePtr);
u32 XFace_accelerate_IsIdle(XFace_accelerate *InstancePtr);
u32 XFace_accelerate_IsReady(XFace_accelerate *InstancePtr);
void XFace_accelerate_EnableAutoRestart(XFace_accelerate *InstancePtr);
void XFace_accelerate_DisableAutoRestart(XFace_accelerate *InstancePtr);

void XFace_accelerate_Set_input_r(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_input_r(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_stem_weights(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_stem_weights(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_stem_bias(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_stem_bias(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_block1_weights(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_block1_weights(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_block1_bias(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_block1_bias(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_block2_weights(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_block2_weights(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_block2_bias(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_block2_bias(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_head0_weights(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_head0_weights(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_head0_bias(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_head0_bias(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_head1_weights(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_head1_weights(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_head1_bias(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_head1_bias(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_scratch1(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_scratch1(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_scratch2(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_scratch2(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_scratch3(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_scratch3(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_scratch4(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_scratch4(XFace_accelerate *InstancePtr);
void XFace_accelerate_Set_output_r(XFace_accelerate *InstancePtr, u64 Data);
u64 XFace_accelerate_Get_output_r(XFace_accelerate *InstancePtr);

void XFace_accelerate_InterruptGlobalEnable(XFace_accelerate *InstancePtr);
void XFace_accelerate_InterruptGlobalDisable(XFace_accelerate *InstancePtr);
void XFace_accelerate_InterruptEnable(XFace_accelerate *InstancePtr, u32 Mask);
void XFace_accelerate_InterruptDisable(XFace_accelerate *InstancePtr, u32 Mask);
void XFace_accelerate_InterruptClear(XFace_accelerate *InstancePtr, u32 Mask);
u32 XFace_accelerate_InterruptGetEnabled(XFace_accelerate *InstancePtr);
u32 XFace_accelerate_InterruptGetStatus(XFace_accelerate *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
