/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.commands.wpilibj2.pidsubsystem;

import edu.wpi.first.wpilibj.controller.PIDController;
import edu.wpi.first.wpilibj2.command.PIDSubsystem;

public class ReplaceMePIDSubsystem extends PIDSubsystem {
  public ReplaceMePIDSubsystem() {
    super(new PIDController(0, 0, 0));
  }

  @Override
  public void useOutput(double output) {
    // Use the output here
  }

  @Override
  public double getSetpoint() {
    // Return the setpoint here
    return 0;
  }

  @Override
  public double getMeasurement() {
    // Return the process variable measurement here
    return 0;
  }
}
