/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "subsystems/Claw.h"

#include <frc2/command/SendableCommandBase.h>
#include <frc2/command/CommandHelper.h>

/**
 * Opens the claw for one second. Real robots should use sensors, stalling
 * motors is BAD!
 */
class CloseClaw : public frc2::CommandHelper<frc2::SendableCommandBase, CloseClaw> {
 public:
  CloseClaw(Claw* claw);
  void Initialize() override;
  bool IsFinished() override;
  void End(bool interrupted) override;
 private:
  Claw* m_claw;
};
