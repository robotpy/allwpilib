// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj.examples.pacgoat.subsystems;

import edu.wpi.first.wpilibj.AnalogGyro;
import edu.wpi.first.wpilibj.Encoder;
import edu.wpi.first.wpilibj.PWMSparkMax;
import edu.wpi.first.wpilibj.SpeedControllerGroup;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.examples.pacgoat.Constants.DriveConstants;
import edu.wpi.first.wpilibj2.command.SubsystemBase;

public class Drivetrain extends SubsystemBase {
  private final PWMSparkMax m_frontLeft = new PWMSparkMax(DriveConstants.kFLPort);
  private final PWMSparkMax m_frontRight = new PWMSparkMax(DriveConstants.kFRPort);
  private final PWMSparkMax m_rearLeft = new PWMSparkMax(DriveConstants.kRLPort);
  private final PWMSparkMax m_rearRight = new PWMSparkMax(DriveConstants.kRRPort);

  private final SpeedControllerGroup m_leftMotors =
      new SpeedControllerGroup(m_frontLeft, m_rearLeft);
  private final SpeedControllerGroup m_rightMotors =
      new SpeedControllerGroup(m_frontRight, m_rearRight);

  private final DifferentialDrive m_drive = new DifferentialDrive(m_leftMotors, m_rightMotors);

  private final Encoder m_leftEncoder =
      new Encoder(DriveConstants.kLEncoderPorts[0], DriveConstants.kLEncoderPorts[1]);
  private final Encoder m_rightEncoder =
      new Encoder(DriveConstants.kREncoderPorts[0], DriveConstants.kREncoderPorts[1]);

  private final AnalogGyro m_gyro = new AnalogGyro(DriveConstants.kGyroPort);

  public Drivetrain() {
    addChild("Front Left", m_frontLeft);
    addChild("Front Right", m_frontRight);
    addChild("Rear Left", m_rearLeft);
    addChild("Rear Right", m_rearRight);

    m_drive.setSafetyEnabled(true);
    m_drive.setExpiration(0.1);
    m_drive.setMaxOutput(1.0);

    m_leftEncoder.setDistancePerPulse(DriveConstants.kDistancePerPulse);
    m_rightEncoder.setDistancePerPulse(DriveConstants.kDistancePerPulse);

    addChild("Left Encoder", m_leftEncoder);
    addChild("Right Encoder", m_rightEncoder);
    addChild("Gyro", m_gyro);
  }

  /**
   * Tank drive using individual joystick axes.
   *
   * @param left The left axis value.
   * @param right The right axis value.
   */
  public void tankDrive(double left, double right) {
    m_drive.tankDrive(left, right);
  }

  /** Stop the drivetrain from moving. */
  public void stop() {
    m_drive.tankDrive(0, 0);
  }

  /**
   * Returns the current angle of the robot as reported by the gyro.
   *
   * @return The current angle of the robot as reported by the gyro.
   */
  public double getAngle() {
    return m_gyro.getAngle();
  }
}
