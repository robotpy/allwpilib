// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj;

import edu.wpi.first.util.sendable.Sendable;
import edu.wpi.first.util.sendable.SendableBuilder;
import edu.wpi.first.util.sendable.SendableRegistry;
import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import java.util.Arrays;

/**
 * Allows multiple {@link SpeedController} objects to be linked together.
 *
 * @deprecated Use {@link edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup}.
 */
@Deprecated(since = "2022", forRemoval = true)
@SuppressWarnings("removal")
public class SpeedControllerGroup implements MotorController, Sendable, AutoCloseable {
  private boolean m_isInverted;
  private final SpeedController[] m_speedControllers;
  private static int instances;

  /**
   * Create a new SpeedControllerGroup with the provided SpeedControllers.
   *
   * @param speedController The first SpeedController to add.
   * @param speedControllers The SpeedControllers to add
   */
  public SpeedControllerGroup(
      SpeedController speedController, SpeedController... speedControllers) {
    m_speedControllers = new SpeedController[speedControllers.length + 1];
    m_speedControllers[0] = speedController;
    System.arraycopy(speedControllers, 0, m_speedControllers, 1, speedControllers.length);
    init();
  }

  public SpeedControllerGroup(SpeedController[] speedControllers) {
    m_speedControllers = Arrays.copyOf(speedControllers, speedControllers.length);
    init();
  }

  private void init() {
    for (SpeedController controller : m_speedControllers) {
      SendableRegistry.addChild(this, controller);
    }
    instances++;
    SendableRegistry.addLW(this, "MotorControllerGroup", instances);
  }

  @Override
  public void close() {
    SendableRegistry.remove(this);
  }

  @Override
  public void set(double speed) {
    for (SpeedController speedController : m_speedControllers) {
      speedController.set(m_isInverted ? -speed : speed);
    }
  }

  @Override
  public double get() {
    if (m_speedControllers.length > 0) {
      return m_speedControllers[0].get() * (m_isInverted ? -1 : 1);
    }
    return 0.0;
  }

  @Override
  public void setInverted(boolean isInverted) {
    m_isInverted = isInverted;
  }

  @Override
  public boolean getInverted() {
    return m_isInverted;
  }

  @Override
  public void disable() {
    for (SpeedController speedController : m_speedControllers) {
      speedController.disable();
    }
  }

  @Override
  public void stopMotor() {
    for (SpeedController speedController : m_speedControllers) {
      speedController.stopMotor();
    }
  }

  @Override
  public void initSendable(SendableBuilder builder) {
    builder.setSmartDashboardType("Motor Controller");
    builder.setActuator(true);
    builder.setSafeState(this::stopMotor);
    builder.addDoubleProperty("Value", this::get, this::set);
  }
}
