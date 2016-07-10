/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2016. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "Types.h"

enum HAL_AccelerometerRange {
  HAL_AccelerometerRange_k2G = 0,
  HAL_AccelerometerRange_k4G = 1,
  HAL_AccelerometerRange_k8G = 2,
};

extern "C" {
void HAL_SetAccelerometerActive(HAL_Bool);
void HAL_SetAccelerometerRange(HAL_AccelerometerRange);
float HAL_GetAccelerometerX();
float HAL_GetAccelerometerY();
float HAL_GetAccelerometerZ();
}
