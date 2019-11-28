/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/DriveSubsystem.h"

#include <frc/geometry/Rotation2d.h>
#include <units/units.h>

#include "Constants.h"

using namespace DriveConstants;

DriveSubsystem::DriveSubsystem()
    : m_frontLeft{kFrontLeftMotorPort},
      m_rearLeft{kRearLeftMotorPort},
      m_frontRight{kFrontRightMotorPort},
      m_rearRight{kRearRightMotorPort},

      m_frontLeftEncoder{kFrontLeftEncoderPorts[0], kFrontLeftEncoderPorts[1],
                         kFrontLeftEncoderReversed},
      m_rearLeftEncoder{kRearLeftEncoderPorts[0], kRearLeftEncoderPorts[1],
                        kRearLeftEncoderReversed},
      m_frontRightEncoder{kFrontRightEncoderPorts[0],
                          kFrontRightEncoderPorts[1],
                          kFrontRightEncoderReversed},
      m_rearRightEncoder{kRearRightEncoderPorts[0], kRearRightEncoderPorts[1],
                         kRearRightEncoderReversed},

      m_odometry{kDriveKinematics,
                 frc::Rotation2d(units::degree_t(GetHeading())),
                 frc::Pose2d()} {
  // Set the distance per pulse for the encoders
  m_frontLeftEncoder.SetDistancePerPulse(kEncoderDistancePerPulse);
  m_rearLeftEncoder.SetDistancePerPulse(kEncoderDistancePerPulse);
  m_frontRightEncoder.SetDistancePerPulse(kEncoderDistancePerPulse);
  m_rearRightEncoder.SetDistancePerPulse(kEncoderDistancePerPulse);
}

void DriveSubsystem::Periodic() {
  // Implementation of subsystem periodic method goes here.
  m_odometry.Update(
      frc::Rotation2d(units::degree_t(GetHeading())),
      frc::MecanumDriveWheelSpeeds{
          units::meters_per_second_t(m_frontLeftEncoder.GetRate()),
          units::meters_per_second_t(m_rearLeftEncoder.GetRate()),
          units::meters_per_second_t(m_frontRightEncoder.GetRate()),
          units::meters_per_second_t(m_rearRightEncoder.GetRate())});
}

void DriveSubsystem::Drive(double xSpeed, double ySpeed, double rot,
                           bool feildRelative) {
  if (feildRelative) {
    m_drive.DriveCartesian(ySpeed, xSpeed, rot, -m_gyro.GetAngle());
  } else {
    m_drive.DriveCartesian(ySpeed, xSpeed, rot);
  }
}

void DriveSubsystem::SetSpeedControllersVolts(units::volt_t frontLeftPower,
                                              units::volt_t rearLeftPower,
                                              units::volt_t frontRightPower,
                                              units::volt_t rearRightPower) {
  m_frontLeft.SetVoltage(frontLeftPower);
  m_rearLeft.SetVoltage(rearLeftPower);
  m_frontRight.SetVoltage(frontRightPower);
  m_rearRight.SetVoltage(rearRightPower);
}

void DriveSubsystem::ResetEncoders() {
  m_frontLeftEncoder.Reset();
  m_rearLeftEncoder.Reset();
  m_frontRightEncoder.Reset();
  m_rearRightEncoder.Reset();
}

frc::Encoder& DriveSubsystem::GetFrontLeftEncoder() {
  return m_frontLeftEncoder;
}

frc::Encoder& DriveSubsystem::GetRearLeftEncoder() { return m_rearLeftEncoder; }

frc::Encoder& DriveSubsystem::GetFrontRightEncoder() {
  return m_frontRightEncoder;
}

frc::Encoder& DriveSubsystem::GetRearRightEncoder() {
  return m_rearRightEncoder;
}

frc::MecanumDriveWheelSpeeds DriveSubsystem::getCurrentWheelSpeeds() {
  return (frc::MecanumDriveWheelSpeeds{
      units::meters_per_second_t(m_frontLeftEncoder.GetRate()),
      units::meters_per_second_t(m_rearLeftEncoder.GetRate()),
      units::meters_per_second_t(m_frontRightEncoder.GetRate()),
      units::meters_per_second_t(m_rearRightEncoder.GetRate())});
}

void DriveSubsystem::SetMaxOutput(double maxOutput) {
  m_drive.SetMaxOutput(maxOutput);
}

double DriveSubsystem::GetHeading() {
  return std::remainder(m_gyro.GetAngle(), 360) * (kGyroReversed ? -1. : 1.);
}

void DriveSubsystem::ZeroHeading() { m_gyro.Reset(); }

double DriveSubsystem::GetTurnRate() {
  return m_gyro.GetRate() * (kGyroReversed ? -1. : 1.);
}

frc::Pose2d DriveSubsystem::GetPose() { return m_odometry.GetPose(); }

void DriveSubsystem::ResetOdometry(frc::Pose2d pose) {
  m_odometry.ResetPosition(pose,
                           frc::Rotation2d(units::degree_t(GetHeading())));
}
