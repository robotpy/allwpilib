/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008-2017. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "PIDSource.h"

/**
 * Set which parameter you are using as a process control variable.
 *
 * @param pidSource An enum to select the parameter.
 */
void frc::PIDSource::SetPIDSourceType(PIDSourceType pidSource) {
  m_pidSource = pidSource;
}

frc::PIDSourceType frc::PIDSource::GetPIDSourceType() const {
  return m_pidSource;
}
