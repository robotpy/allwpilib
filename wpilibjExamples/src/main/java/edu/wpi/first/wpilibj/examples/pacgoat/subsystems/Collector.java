// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj.examples.pacgoat.subsystems;

import edu.wpi.first.wpilibj.DigitalInput;
import edu.wpi.first.wpilibj.PWMSparkMax;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.examples.pacgoat.Constants;
import edu.wpi.first.wpilibj.examples.pacgoat.Constants.CollectorConstants;
import edu.wpi.first.wpilibj2.command.SubsystemBase;

public class Collector extends SubsystemBase {
  private final PWMSparkMax m_roller = new PWMSparkMax(CollectorConstants.kMotorPort);

  private final DigitalInput m_ballDetector =
      new DigitalInput(CollectorConstants.kBallDetectorPort);
  private final DigitalInput m_openDetector =
      new DigitalInput(CollectorConstants.kOpenDetectorPort);

  private final Solenoid m_piston =
      new Solenoid(Constants.kPCMId, CollectorConstants.kPistonSolenoidId);

  public Collector() {
    addChild("Roller Motor", m_roller);
    addChild("Ball Detector", m_ballDetector);
    addChild("Open Detector", m_openDetector);
    addChild("Piston", m_piston);

    m_roller.setInverted(true);
  }

  /**
   * Returns whether the robot has a ball.
   *
   * @return Whether the robot has a ball.
   */
  public boolean hasBall() {
    return m_ballDetector.get();
  }

  /**
   * Returns whether the claw is open.
   *
   * @return Whether the claw is open.
   */
  public boolean isOpen() {
    return m_openDetector.get();
  }

  /**
   * Sets the speed of the roller motor.
   *
   * @param speed The desired speed of the roller motor.
   */
  public void setSpeed(double speed) {
    m_roller.set(speed);
  }

  /** Stops the rollers from moving. */
  public void stop() {
    m_roller.set(0);
  }

  /** Opens the claw. */
  public void open() {
    m_piston.set(true);
  }

  /** Closes the claw. */
  public void close() {
    m_piston.set(false);
  }
}
