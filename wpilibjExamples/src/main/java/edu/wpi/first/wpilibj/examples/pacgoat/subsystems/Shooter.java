// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj.examples.pacgoat.subsystems;

import edu.wpi.first.wpilibj.DigitalInput;
import edu.wpi.first.wpilibj.DoubleSolenoid;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.examples.pacgoat.Constants;
import edu.wpi.first.wpilibj.examples.pacgoat.Constants.ShooterConstants;
import edu.wpi.first.wpilibj2.command.SubsystemBase;

public class Shooter extends SubsystemBase {
  DoubleSolenoid m_piston1 =
      new DoubleSolenoid(
          Constants.kPCMId, ShooterConstants.kPiston1Ids[0], ShooterConstants.kPiston1Ids[1]);
  DoubleSolenoid m_piston2 =
      new DoubleSolenoid(
          Constants.kPCMId, ShooterConstants.kPiston2Ids[0], ShooterConstants.kPiston2Ids[1]);

  Solenoid m_latchPiston = new Solenoid(Constants.kPCMId, ShooterConstants.kLatchPistonId);

  DigitalInput m_piston1ReedSwitchFront =
      new DigitalInput(ShooterConstants.kPiston1ReedSwitchFrontPort);
  DigitalInput m_piston1ReedSwitchBack =
      new DigitalInput(ShooterConstants.kPiston1ReedSwitchBackPort);

  DigitalInput m_hotGoalSensor = new DigitalInput(ShooterConstants.kHotGoalSensorPort);

  public Shooter() {
    addChild("Hot Goal Sensor", m_hotGoalSensor);
    addChild("Piston 1 Reed Switch Front", m_piston1ReedSwitchFront);
    addChild("Piston 1 Reed Switch Back", m_piston1ReedSwitchBack);
    addChild("Latch Piston", m_latchPiston);
  }

  /** Extend both pistons to shoot. */
  public void extendBoth() {
    m_piston1.set(DoubleSolenoid.Value.kForward);
    m_piston2.set(DoubleSolenoid.Value.kForward);
  }

  /** Retract both pistons to prepare to shoot. */
  public void retractBoth() {
    m_piston1.set(DoubleSolenoid.Value.kReverse);
    m_piston2.set(DoubleSolenoid.Value.kReverse);
  }

  /** Extend piston 1 to shoot. */
  public void extend1() {
    m_piston1.set(DoubleSolenoid.Value.kForward);
  }

  /** Retract piston 1 to prepare to shoot. */
  public void retract1() {
    m_piston1.set(DoubleSolenoid.Value.kReverse);
  }

  /** Extend piston 2 to shoot. */
  public void extend2() {
    m_piston2.set(DoubleSolenoid.Value.kForward);
  }

  /** Retract piston 2 to prepare to shoot. */
  public void retract2() {
    m_piston2.set(DoubleSolenoid.Value.kReverse);
  }

  /**
   * Turns off piston 1. This won't actuate anything because double solenoids preserve their state
   * when turned off. This should be called in order to reduce the amount of time that the coils are
   * powered.
   */
  public void off1() {
    m_piston1.set(DoubleSolenoid.Value.kOff);
  }

  /**
   * Turns off piston 2. This won't actuate anything because double solenoids preserve their state
   * when turned off. This should be called in order to reduce the amount of time that the coils are
   * powered.
   */
  public void off2() {
    m_piston2.set(DoubleSolenoid.Value.kOff);
  }

  /** Release the latch so we can shoot. */
  public void unlatch() {
    m_latchPiston.set(true);
  }

  /** Latch so that pressure can build up and we aren't limited by air flow. */
  public void latch() {
    m_latchPiston.set(false);
  }

  /** Toggles the latch position. */
  public void toggleLatchPosition() {
    m_latchPiston.toggle();
  }

  /**
   * Returns whether piston 1 is extended after shooting.
   *
   * @return Whether piston 1 is extended.
   */
  public boolean isPiston1Extended() {
    return !m_piston1ReedSwitchFront.get();
  }

  /**
   * Returns whether piston 1 is retracted before shooting.
   *
   * @return Whether piston 1 is retracted.
   */
  public boolean isPiston1Retracted() {
    return !m_piston1ReedSwitchBack.get();
  }

  /** Turns off both double solenoids. */
  public void offBoth() {
    off1();
    off2();
  }

  /**
   * Returns whether the goal is hot as read by the banner sensor.
   *
   * @return Whether the goal is hot.
   */
  public boolean isGoalHot() {
    return m_hotGoalSensor.get();
  }
}
