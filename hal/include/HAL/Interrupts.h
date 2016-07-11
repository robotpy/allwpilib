/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2016. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <stdint.h>

#include "AnalogTrigger.h"
#include "Types.h"

extern "C" {
typedef void (*InterruptHandlerFunction)(uint32_t interruptAssertedMask,
                                         void* param);

HAL_InterruptHandle HAL_InitializeInterrupts(HAL_Bool watcher, int32_t* status);
void HAL_CleanInterrupts(HAL_InterruptHandle interrupt_handle, int32_t* status);

int64_t HAL_WaitForInterrupt(HAL_InterruptHandle interrupt_handle,
                             double timeout, HAL_Bool ignorePrevious,
                             int32_t* status);
void HAL_EnableInterrupts(HAL_InterruptHandle interrupt_handle,
                          int32_t* status);
void HAL_DisableInterrupts(HAL_InterruptHandle interrupt_handle,
                           int32_t* status);
double HAL_ReadInterruptRisingTimestamp(HAL_InterruptHandle interrupt_handle,
                                        int32_t* status);
double HAL_ReadInterruptFallingTimestamp(HAL_InterruptHandle interrupt_handle,
                                         int32_t* status);
void HAL_RequestInterrupts(HAL_InterruptHandle interrupt_handle,
                           HAL_Handle digitalSourceHandle,
                           HAL_AnalogTriggerType analogTriggerType,
                           int32_t* status);
void HAL_AttachInterruptHandler(HAL_InterruptHandle interrupt_handle,
                                InterruptHandlerFunction handler, void* param,
                                int32_t* status);
void HAL_SetInterruptUpSourceEdge(HAL_InterruptHandle interrupt_handle,
                                  HAL_Bool risingEdge, HAL_Bool fallingEdge,
                                  int32_t* status);
}
