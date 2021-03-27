// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj.examples.pacgoat.subsystems;

import edu.wpi.first.wpilibj.AnalogInput;
import edu.wpi.first.wpilibj.RobotBase;
import edu.wpi.first.wpilibj.examples.pacgoat.Constants.PneumaticsConstants;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj2.command.SubsystemBase;

public class Pneumatics extends SubsystemBase {
  private final AnalogInput m_pressureSensor =
      new AnalogInput(PneumaticsConstants.kPressureSensorPort);

  public Pneumatics() {
    addChild("Pressure Sensor", m_pressureSensor);
  }

  /**
   * Returns whether the system is pressurized.
   *
   * @return Whether the system is pressurized.
   */
  public boolean isPressurized() {
    // The pressure sensor does not exist outside of real hardware so we can return true in sim.
    if (RobotBase.isReal()) {
      return PneumaticsConstants.kMaxPressure <= m_pressureSensor.getVoltage();
    } else {
      return true;
    }
  }

  /** Writes the pressure value to SmartDashboard. */
  public void writePressure() {
    SmartDashboard.putNumber("Pressure", m_pressureSensor.getVoltage());
  }
}
