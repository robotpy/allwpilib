// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj.examples.pacgoat.subsystems;

import edu.wpi.first.wpilibj.AnalogPotentiometer;
import edu.wpi.first.wpilibj.DigitalInput;
import edu.wpi.first.wpilibj.PWMSparkMax;
import edu.wpi.first.wpilibj.controller.PIDController;
import edu.wpi.first.wpilibj.examples.pacgoat.Constants.PivotConstants;
import edu.wpi.first.wpilibj.interfaces.Potentiometer;
import edu.wpi.first.wpilibj2.command.PIDSubsystem;

public class Pivot extends PIDSubsystem {
  private final PWMSparkMax m_pivot = new PWMSparkMax(PivotConstants.kMotorPort);

  private final DigitalInput m_upperLimitSwitch =
      new DigitalInput(PivotConstants.kUpperLimitSwitchPort);
  private final DigitalInput m_lowerLimitSwitch =
      new DigitalInput(PivotConstants.kLowerLimitSwitchPort);

  private final Potentiometer m_pot = new AnalogPotentiometer(PivotConstants.kPotentiometerPort);

  public Pivot() {
    super(new PIDController(PivotConstants.kP, PivotConstants.kI, PivotConstants.kD));
    getController().setTolerance(0.005);
    getController().disableContinuousInput();

    addChild("Pivot Motor", m_pivot);
    addChild("Upper Limit Switch", m_upperLimitSwitch);
    addChild("Lower Limit Switch", m_lowerLimitSwitch);
    addChild("Potentiometer", (AnalogPotentiometer) m_pot);
  }

  @Override
  protected double getMeasurement() {
    return m_pot.get();
  }

  @Override
  protected void useOutput(double output, double setpoint) {
    m_pivot.set(output);
  }

  /**
   * Returns whether the pivot is at its upper limit.
   *
   * @return Whether the pivot is at its upper limit.
   */
  public boolean isAtUpperLimit() {
    return m_upperLimitSwitch.get();
  }

  /**
   * Returns whether the pivot is at its lower limit.
   *
   * @return Whether the pivot is at its lower limit.
   */
  public boolean isAtLowerLimit() {
    return m_lowerLimitSwitch.get();
  }

  /**
   * Returns the angle of the pivot.
   *
   * @return The angle of the pivot.
   */
  public double getAngle() {
    return m_pot.get();
  }
}
