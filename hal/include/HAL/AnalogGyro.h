/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2016. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <stdint.h>

#include "Types.h"

extern "C" {
HAL_GyroHandle HAL_InitializeAnalogGyro(HAL_AnalogInputHandle handle,
                                        int32_t* status);
void HAL_SetupAnalogGyro(HAL_GyroHandle handle, int32_t* status);
void HAL_FreeAnalogGyro(HAL_GyroHandle handle);
void HAL_SetAnalogGyroParameters(HAL_GyroHandle handle,
                                 float voltsPerDegreePerSecond, float offset,
                                 int32_t center, int32_t* status);
void HAL_SetAnalogGyroVoltsPerDegreePerSecond(HAL_GyroHandle handle,
                                              float voltsPerDegreePerSecond,
                                              int32_t* status);
void HAL_ResetAnalogGyro(HAL_GyroHandle handle, int32_t* status);
void HAL_CalibrateAnalogGyro(HAL_GyroHandle handle, int32_t* status);
void HAL_SetAnalogGyroDeadband(HAL_GyroHandle handle, float volts,
                               int32_t* status);
float HAL_GetAnalogGyroAngle(HAL_GyroHandle handle, int32_t* status);
float HAL_GetAnalogGyroRate(HAL_GyroHandle handle, int32_t* status);
float HAL_GetAnalogGyroOffset(HAL_GyroHandle handle, int32_t* status);
int32_t HAL_GetAnalogGyroCenter(HAL_GyroHandle handle, int32_t* status);
}
