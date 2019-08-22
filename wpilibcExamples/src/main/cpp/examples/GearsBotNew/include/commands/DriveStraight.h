/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc2/command/SynchronousPIDCommand.h>
#include <frc2/command/CommandHelper.h>

/**
 * Drive the given distance straight (negative values go backwards).
 * Uses a local PID controller to run a simple PID loop that is only
 * enabled while this command is running. The input is the averaged
 * values of the left and right encoders.
 */
class DriveStraight : public frc2::CommandHelper<frc2::SynchronousPIDCommand, DriveStraight> {
public:
  explicit DriveStraight(double distance, DriveTrain* drivetrain);
  void Initialize() override;
  bool IsFinished() override;

 private:
  DriveTrain* m_drivetrain;
};
