/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.examples.mecanumbot;

import edu.wpi.first.wpilibj.GenericHID;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.XboxController;

import static edu.wpi.first.wpilibj.examples.mecanumbot.Constants.DriveConstants.kMaxAngularSpeed;
import static edu.wpi.first.wpilibj.examples.mecanumbot.Constants.DriveConstants.kMaxSpeed;

public class Robot extends TimedRobot {
  private final XboxController m_controller = new XboxController(0);
  private final Drivetrain m_mecanum = new Drivetrain();

  @Override
  public void autonomousPeriodic() {
    driveWithJoystick(false);
    m_mecanum.updateOdometry();
  }

  @Override
  public void teleopPeriodic() {
    driveWithJoystick(true);
  }

  private void driveWithJoystick(boolean fieldRelative) {
    // Get the x speed. We are inverting this because Xbox controllers return
    // negative values when we push forward.
    final var xSpeed = -m_controller.getY(GenericHID.Hand.kLeft) * kMaxSpeed;

    // Get the y speed or sideways/strafe speed. We are inverting this because
    // we want a positive value when we pull to the left. Xbox controllers
    // return positive values when you pull to the right by default.
    final var ySpeed = -m_controller.getX(GenericHID.Hand.kLeft) * kMaxSpeed;

    // Get the rate of angular rotation. We are inverting this because we want a
    // positive value when we pull to the left (remember, CCW is positive in
    // mathematics). Xbox controllers return positive values when you pull to
    // the right by default.
    final var rot = -m_controller.getX(GenericHID.Hand.kRight) * kMaxAngularSpeed;

    m_mecanum.drive(xSpeed, ySpeed, rot, fieldRelative);
  }
}
