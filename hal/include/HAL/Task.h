/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2016. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <pthread.h>
#include <stdint.h>

typedef pthread_t* TASK;

#ifdef __cplusplus
extern "C" {
#endif
// Note: These constants used to be declared extern and were defined in
// Task.cpp. This caused issues with the JNI bindings for java, and so the
// instantiations were moved here.
const int32_t HAL_TaskLib_ILLEGAL_PRIORITY = 22;  // 22 is EINVAL

int32_t HAL_VerifyTaskID(TASK task);

// valid priority [1..99]
int32_t HAL_SetTaskPriority(TASK task, int32_t priority);

int32_t HAL_GetTaskPriority(TASK task, int32_t* priority);
#ifdef __cplusplus
}
#endif
