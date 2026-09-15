// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#ifdef SDT
#include "xparameters.h"
#endif
#include "xface_accelerate.h"

extern XFace_accelerate_Config XFace_accelerate_ConfigTable[];

#ifdef SDT
XFace_accelerate_Config *XFace_accelerate_LookupConfig(UINTPTR BaseAddress) {
	XFace_accelerate_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XFace_accelerate_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XFace_accelerate_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XFace_accelerate_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XFace_accelerate_Initialize(XFace_accelerate *InstancePtr, UINTPTR BaseAddress) {
	XFace_accelerate_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XFace_accelerate_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XFace_accelerate_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XFace_accelerate_Config *XFace_accelerate_LookupConfig(u16 DeviceId) {
	XFace_accelerate_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XFACE_ACCELERATE_NUM_INSTANCES; Index++) {
		if (XFace_accelerate_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XFace_accelerate_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XFace_accelerate_Initialize(XFace_accelerate *InstancePtr, u16 DeviceId) {
	XFace_accelerate_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XFace_accelerate_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XFace_accelerate_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

