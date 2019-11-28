/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc2/command/PIDSubsystem.h"

using namespace frc2;

PIDSubsystem::PIDSubsystem(PIDController controller)
    : m_controller{controller} {}

void PIDSubsystem::Periodic() {
  if (m_enabled) {
    UseOutput(m_controller.Calculate(GetMeasurement(), m_setpoint), m_setpoint);
  }
}

void PIDSubsystem::SetSetpoint(double setpoint) { m_setpoint = setpoint; }

void PIDSubsystem::Enable() {
  m_controller.Reset();
  m_enabled = true;
}

void PIDSubsystem::Disable() {
  UseOutput(0, 0);
  m_enabled = false;
}

bool PIDSubsystem::IsEnabled() { return m_enabled; }

PIDController& PIDSubsystem::GetController() { return m_controller; }
