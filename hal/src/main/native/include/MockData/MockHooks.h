/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2017. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

extern "C" {
void HALSIM_WaitForProgramStart(void);
void HALSIM_SetProgramStarted(void);
void HALSIM_RestartTiming(void);
}
