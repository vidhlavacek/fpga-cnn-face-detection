# ==============================================================
# Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
# Tool Version Limit: 2024.05
# Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
# Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
# 
# ==============================================================
cmake_minimum_required(VERSION 3.15)
project(face_accelerate)
find_package(common)
collector_create (PROJECT_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}")
collect (PROJECT_LIB_SOURCES xface_accelerate.c)
collect (PROJECT_LIB_SOURCES xface_accelerate_sinit.c)
collect (PROJECT_LIB_SOURCES xface_accelerate_linux.c)
collect (PROJECT_LIB_SOURCES xface_accelerate_g.c)
collector_list (_sources PROJECT_LIB_SOURCES)
collector_create (PROJECT_LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}")
collect (PROJECT_LIB_HEADERS xface_accelerate.h)
collect (PROJECT_LIB_HEADERS xface_accelerate_hw.h)
collector_list (_headers PROJECT_LIB_HEADERS)
file(COPY ${_headers} DESTINATION ${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/include)
add_library(face_accelerate STATIC ${_sources})
set_target_properties(face_accelerate PROPERTIES LINKER_LANGUAGE C)

