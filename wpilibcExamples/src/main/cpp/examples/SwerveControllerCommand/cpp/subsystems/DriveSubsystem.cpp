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
    : m_frontLeft{kFrontLeftDriveMotorPort,
                  kFrontLeftTurningMotorPort,
                  kFrontLeftDriveEncoderPorts,
                  kFrontLeftTurningEncoderPorts,
                  kFrontLeftDriveEncoderReversed,
                  kFrontLeftTurningEncoderReversed},

      m_rearLeft{
          kRearLeftDriveMotorPort,       kRearLeftTurningMotorPort,
          kRearLeftDriveEncoderPorts,    kRearLeftTurningEncoderPorts,
          kRearLeftDriveEncoderReversed, kRearLeftTurningEncoderReversed},

      m_frontRight{
          kFrontRightDriveMotorPort,       kFrontRightTurningMotorPort,
          kFrontRightDriveEncoderPorts,    kFrontRightTurningEncoderPorts,
          kFrontRightDriveEncoderReversed, kFrontRightTurningEncoderReversed},

      m_rearRight{
          kRearRightDriveMotorPort,       kRearRightTurningMotorPort,
          kRearRightDriveEncoderPorts,    kRearRightTurningEncoderPorts,
          kRearRightDriveEncoderReversed, kRearRightTurningEncoderReversed},

      m_odometry{kDriveKinematics,
                 frc::Rotation2d(units::degree_t(GetHeading())),
                 frc::Pose2d()} {}

void DriveSubsystem::Periodic() {
  // Implementation of subsystem periodic method goes here.
  m_odometry.Update(frc::Rotation2d(units::degree_t(GetHeading())),
                    m_frontLeft.GetState(), m_rearLeft.GetState(),
                    m_frontRight.GetState(), m_rearRight.GetState());
}

void DriveSubsystem::Drive(units::meters_per_second_t xSpeed,
                           units::meters_per_second_t ySpeed,
                           units::radians_per_second_t rot,
                           bool fieldRelative) {
  auto states = kDriveKinematics.ToSwerveModuleStates(
      fieldRelative ? frc::ChassisSpeeds::FromFieldRelativeSpeeds(
                          xSpeed, ySpeed, rot,
                          frc::Rotation2d(units::degree_t(GetHeading())))
                    : frc::ChassisSpeeds{xSpeed, ySpeed, rot});

  kDriveKinematics.NormalizeWheelSpeeds(&states, AutoConstants::kMaxSpeed);

  auto [fl, fr, bl, br] = states;

  m_frontLeft.SetDesiredState(fl);
  m_frontRight.SetDesiredState(fr);
  m_rearLeft.SetDesiredState(bl);
  m_rearRight.SetDesiredState(br);
}

void DriveSubsystem::SetModuleStates(
    std::array<frc::SwerveModuleState, 4> desiredStates) {
  kDriveKinematics.NormalizeWheelSpeeds(&desiredStates,
                                        AutoConstants::kMaxSpeed);
  m_frontLeft.SetDesiredState(desiredStates[0]);
  m_rearLeft.SetDesiredState(desiredStates[1]);
  m_frontRight.SetDesiredState(desiredStates[2]);
  m_rearRight.SetDesiredState(desiredStates[3]);
}

void DriveSubsystem::ResetEncoders() {
  m_frontLeft.ResetEncoders();
  m_rearLeft.ResetEncoders();
  m_frontRight.ResetEncoders();
  m_rearRight.ResetEncoders();
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
