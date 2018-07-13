/*----------------------------------------------------------------------------*/
/* Use and modify this template how you wish, it's public domain.             */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.examples.pacgoat.subsystems;

import edu.wpi.first.wpilibj.AnalogInput;
import edu.wpi.first.wpilibj.command.Subsystem;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import edu.wpi.first.wpilibj.examples.pacgoat.Robot;

/**
 * The Pneumatics subsystem contains a pressure sensor.
 *
 * <p>NOTE: The simulator currently doesn't support the compressor or pressure
 * sensors.
 */
public class Pneumatics extends Subsystem {
  AnalogInput m_pressureSensor = new AnalogInput(3);

  private static final double kMaxPressure = 2.55;

  public Pneumatics() {
    addChild("Pressure Sensor", m_pressureSensor);
  }

  /**
   * No default command.
   */
  @Override
  public void initDefaultCommand() {
  }

  /**
   * Whether or not the system is fully pressurized.
   */
  public boolean isPressurized() {
    if (Robot.isReal()) {
      return kMaxPressure <= m_pressureSensor.getVoltage();
    } else {
      return true; // NOTE: Simulation always has full pressure
    }
  }

  /**
   * Puts the pressure on the SmartDashboard.
   */
  public void writePressure() {
    SmartDashboard.putNumber("Pressure", m_pressureSensor.getVoltage());
  }
}
