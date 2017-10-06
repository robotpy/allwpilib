/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "HAL/HAL.h"
#include "NotifyListener.h"

#ifdef __cplusplus
extern "C" {
#endif

void HALSIM_ResetI2CData(int32_t index);

//////////////////////
// Initialize
//////////////////////
int32_t HALSIM_RegisterI2CInitializedCallback(int32_t index,
                                              HAL_NotifyCallback callback,
                                              void* param,
                                              HAL_Bool initialNotify);
void HALSIM_CancelI2CInitializedCallback(int32_t index, int32_t uid);
HAL_Bool HALSIM_GetI2CInitialized(int32_t index);
void HALSIM_SetI2CInitialized(int32_t index, HAL_Bool initialized);

int32_t HALSIM_RegisterI2CReadCallback(int32_t index,
		HAL_ReadBufferCallback callback,
                                          void* param);
void HALSIM_CancelI2CReadCallback(int32_t index,
                                     int32_t uid);

int32_t HALSIM_RegisterI2CWriteCallback(int32_t index,
		HAL_WriteBufferCallback callback,
                                          void* param);
void HALSIM_CancelI2CWriteCallback(int32_t index,
                                     int32_t uid);

#ifdef __cplusplus
}
#endif
