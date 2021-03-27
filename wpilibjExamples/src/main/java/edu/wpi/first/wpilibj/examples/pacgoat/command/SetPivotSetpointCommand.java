// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj.examples.pacgoat.command;

import edu.wpi.first.wpilibj.examples.pacgoat.subsystems.Pivot;
import edu.wpi.first.wpilibj2.command.CommandBase;

/**
 * Moves the pivot to a given angle. This command finishes when it is within the tolerance, but
 * leaves the PIDController enabled to maintain position.
 */
public class SetPivotSetpointCommand extends CommandBase {
  private final Pivot m_pivot;
  private final double m_setpoint;

  public SetPivotSetpointCommand(Pivot pivot, double setpoint) {
    addRequirements(pivot);

    m_pivot = pivot;
    m_setpoint = setpoint;
  }

  @Override
  public void initialize() {
    m_pivot.enable();
    m_pivot.setSetpoint(m_setpoint);
  }

  @Override
  public boolean isFinished() {
    return m_pivot.getController().atSetpoint();
  }
}
