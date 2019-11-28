/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/TurnToAngle.h"

#include <frc/controller/PIDController.h>

using namespace DriveConstants;

TurnToAngle::TurnToAngle(double targetAngleDegrees, DriveSubsystem* drive)
    : CommandHelper(frc2::PIDController(kTurnP, kTurnI, kTurnD),
                    // Close loop on heading
                    [drive] { return drive->GetHeading(); },
                    // Set reference to target
                    targetAngleDegrees,
                    // Pipe output to turn robot
                    [drive](double output) { drive->ArcadeDrive(0, output); },
                    // Require the drive
                    {drive}) {
  // Set the controller to be continuous (because it is an angle controller)
  m_controller.EnableContinuousInput(-180, 180);
  // Set the controller tolerance - the delta tolerance ensures the robot is
  // stationary at the setpoint before it is considered as having reached the
  // reference
  m_controller.SetTolerance(kTurnToleranceDeg, kTurnRateToleranceDegPerS);

  AddRequirements({drive});
}

bool TurnToAngle::IsFinished() { return m_controller.AtSetpoint(); }
