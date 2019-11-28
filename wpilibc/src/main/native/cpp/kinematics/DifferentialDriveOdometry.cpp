/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/kinematics/DifferentialDriveOdometry.h"

using namespace frc;

DifferentialDriveOdometry::DifferentialDriveOdometry(
    DifferentialDriveKinematics kinematics, const Rotation2d& gyroAngle,
    const Pose2d& initialPose)
    : m_kinematics(kinematics), m_pose(initialPose) {
  m_previousAngle = m_pose.Rotation();
  m_gyroOffset = m_pose.Rotation() - gyroAngle;
}

const Pose2d& DifferentialDriveOdometry::UpdateWithTime(
    units::second_t currentTime, const Rotation2d& gyroAngle,
    const DifferentialDriveWheelSpeeds& wheelSpeeds) {
  units::second_t deltaTime =
      (m_previousTime >= 0_s) ? currentTime - m_previousTime : 0_s;
  m_previousTime = currentTime;

  auto angle = gyroAngle + m_gyroOffset;

  auto [dx, dy, dtheta] = m_kinematics.ToChassisSpeeds(wheelSpeeds);
  static_cast<void>(dtheta);

  auto newPose = m_pose.Exp(
      {dx * deltaTime, dy * deltaTime, (angle - m_previousAngle).Radians()});

  m_previousAngle = angle;
  m_pose = {newPose.Translation(), angle};

  return m_pose;
}

const Pose2d& DifferentialDriveOdometry::Update(const Rotation2d& gyroAngle,
                                                units::meter_t leftDistance,
                                                units::meter_t rightDistance) {
  auto deltaLeftDistance = leftDistance - m_prevLeftDistance;
  auto deltaRightDistance = rightDistance - m_prevRightDistance;

  m_prevLeftDistance = leftDistance;
  m_prevRightDistance = rightDistance;

  auto averageDeltaDistance = (deltaLeftDistance + deltaRightDistance) / 2.0;
  auto angle = gyroAngle + m_gyroOffset;

  auto newPose = m_pose.Exp(
      {averageDeltaDistance, 0_m, (angle - m_previousAngle).Radians()});

  m_previousAngle = angle;
  m_pose = {newPose.Translation(), angle};

  return m_pose;
}
