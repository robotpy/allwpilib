// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj.examples.pacgoat;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.examples.pacgoat.command.SetPivotSetpointCommand;
import edu.wpi.first.wpilibj.examples.pacgoat.subsystems.Collector;
import edu.wpi.first.wpilibj.examples.pacgoat.subsystems.Drivetrain;
import edu.wpi.first.wpilibj.examples.pacgoat.subsystems.Pivot;
import edu.wpi.first.wpilibj.examples.pacgoat.subsystems.Pneumatics;
import edu.wpi.first.wpilibj.examples.pacgoat.subsystems.Shooter;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj2.command.InstantCommand;
import edu.wpi.first.wpilibj2.command.ParallelCommandGroup;
import edu.wpi.first.wpilibj2.command.SequentialCommandGroup;
import edu.wpi.first.wpilibj2.command.WaitCommand;
import edu.wpi.first.wpilibj2.command.WaitUntilCommand;
import edu.wpi.first.wpilibj2.command.button.JoystickButton;

public class RobotContainer {
  // Initialize subsystems.
  private final Drivetrain m_drive = new Drivetrain();
  private final Collector m_collector = new Collector();
  private final Pivot m_pivot = new Pivot();
  private final Shooter m_shooter = new Shooter();
  private final Pneumatics m_pneumatics = new Pneumatics();

  // Create joystick
  private final Joystick m_joystick = new Joystick(Constants.kJoystickPort);

  public RobotContainer() {
    configureButtonBindings();
  }

  public void configureButtonBindings() {
    // Low Goal
    new JoystickButton(m_joystick, 12)
        .whenPressed(
            new SequentialCommandGroup(
                // Set the pivot setpoint and wait until it gets there.
                new SetPivotSetpointCommand(m_pivot, Constants.PivotConstants.kLowGoal),

                // Set the collector speed.
                new InstantCommand(
                    () -> m_collector.setSpeed(Constants.CollectorConstants.kReverse), m_collector),

                // Extend the shooter and then retract it after a second.
                new InstantCommand(m_shooter::extendBoth, m_shooter),
                new WaitCommand(1),
                new InstantCommand(m_shooter::retractBoth, m_shooter)));

    // Collect
    new JoystickButton(m_joystick, 10)
        .whenPressed(
            new SequentialCommandGroup(
                // Set the collector speed.
                new InstantCommand(
                    () -> m_collector.setSpeed(Constants.CollectorConstants.kForward), m_collector),

                // Close the claw and set the pivot setpoint. Cancel this group when we have a ball.
                new WaitUntilCommand(m_collector::hasBall)
                    .deadlineWith(
                        new ParallelCommandGroup(
                            new InstantCommand(m_collector::close, m_collector),
                            new SetPivotSetpointCommand(
                                m_pivot, Constants.PivotConstants.kCollect)))));

    // Pivot Positions.
    new JoystickButton(m_joystick, 11)
        .whenPressed(new SetPivotSetpointCommand(m_pivot, Constants.PivotConstants.kShoot));
    new JoystickButton(m_joystick, 9)
        .whenPressed(new SetPivotSetpointCommand(m_pivot, Constants.PivotConstants.kShootNear));

    // Shoot
    new JoystickButton(m_joystick, 2)
        .and(new JoystickButton(m_joystick, 3))
        .whenActive(
            new SequentialCommandGroup(
                // Wait for pressure to build up.
                new WaitUntilCommand(m_pneumatics::isPressurized),

                // Ensure the collector isn't running.
                new InstantCommand(
                    () -> m_collector.setSpeed(Constants.CollectorConstants.kStop), m_collector),

                // Open the claw and wait until the sensor says it is open.
                new InstantCommand(m_collector::open, m_collector),
                new WaitUntilCommand(m_collector::isOpen),

                // Extend the shooter and then retract it after a second.
                new InstantCommand(m_shooter::extendBoth, m_shooter),
                new WaitCommand(1),
                new InstantCommand(m_shooter::retractBoth, m_shooter)));

    // SmartDashboard Buttons
    SmartDashboard.putData(
        "Start Rollers",
        new InstantCommand(
            () -> m_collector.setSpeed(Constants.CollectorConstants.kForward), m_collector));
    SmartDashboard.putData(
        "Stop Rollers",
        new InstantCommand(
            () -> m_collector.setSpeed(Constants.CollectorConstants.kStop), m_collector));
    SmartDashboard.putData(
        "Reverse Rollers",
        new InstantCommand(
            () -> m_collector.setSpeed(Constants.CollectorConstants.kReverse), m_collector));
  }
}
