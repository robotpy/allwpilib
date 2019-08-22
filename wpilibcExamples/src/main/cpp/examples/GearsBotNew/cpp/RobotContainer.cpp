/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "RobotContainer.h"

#include <frc2/command/button/JoystickButton.h>

#include <frc/smartdashboard/SmartDashboard.h>

#include "commands/CloseClaw.h"
#include "commands/OpenClaw.h"
#include "commands/Pickup.h"
#include "commands/Place.h"
#include "commands/PrepareToPickup.h"
#include "commands/SetElevatorSetpoint.h"

RobotContainer::RobotContainer()
    : m_autonomousCommand(&m_claw, &m_wrist, &m_elevator, &m_drivetrain) {
  frc::SmartDashboard::PutData(&m_drivetrain);
  frc::SmartDashboard::PutData(&m_elevator);
  frc::SmartDashboard::PutData(&m_wrist);
  frc::SmartDashboard::PutData(&m_claw);

  m_claw.Log();
  m_wrist.Log();
  m_elevator.Log();
  m_drivetrain.Log();

  // Configure the button bindings
  ConfigureButtonBindings();
}

void RobotContainer::ConfigureButtonBindings() {
  // Configure your button bindings here
  frc2::JoystickButton m_dUp{&m_joy, 5};
  frc2::JoystickButton m_dRight{&m_joy, 6};
  frc2::JoystickButton m_dDown{&m_joy, 7};
  frc2::JoystickButton m_dLeft{&m_joy, 8};
  frc2::JoystickButton m_l2{&m_joy, 9};
  frc2::JoystickButton m_r2{&m_joy, 10};
  frc2::JoystickButton m_l1{&m_joy, 11};
  frc2::JoystickButton m_r1{&m_joy, 12};

  m_dUp.WhenPressed(new SetElevatorSetpoint(0.2, &m_elevator));
  m_dDown.WhenPressed(new SetElevatorSetpoint(-0.2, &m_elevator));
  m_dRight.WhenPressed(new CloseClaw(&m_claw));
  m_dLeft.WhenPressed(new OpenClaw(&m_claw));

  m_r1.WhenPressed(new PrepareToPickup(&m_claw, &m_wrist, &m_elevator));
  m_r2.WhenPressed(new Pickup(&m_claw, &m_wrist, &m_elevator));
  m_l1.WhenPressed(new Place(&m_claw, &m_wrist, &m_elevator));
  m_l2.WhenPressed(
      new Autonomous(&m_claw, &m_wrist, &m_elevator, &m_drivetrain));
}

frc2::Command* RobotContainer::GetAutonomousCommand() {
  // An example command will be run in autonomous
  return &m_autonomousCommand;
}
