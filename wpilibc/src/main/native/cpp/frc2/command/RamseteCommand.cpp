/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc2/command/RamseteCommand.h"

using namespace frc2;
using namespace units;

template <typename T>
int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

RamseteCommand::RamseteCommand(
    frc::Trajectory trajectory, std::function<frc::Pose2d()> pose,
    frc::RamseteController follower, units::voltage::volt_t ks,
    units::unit_t<voltsecondspermeter> kv,
    units::unit_t<voltsecondssquaredpermeter> ka,
    frc::DifferentialDriveKinematics kinematics,
    std::function<units::meters_per_second_t()> leftSpeed,
    std::function<units::meters_per_second_t()> rightSpeed,
    frc2::PIDController leftController, frc2::PIDController rightController,
    std::function<void(voltage::volt_t, voltage::volt_t)> output)
    : m_trajectory(trajectory),
      m_pose(pose),
      m_follower(follower),
      m_ks(ks),
      m_kv(kv),
      m_ka(ka),
      m_kinematics(kinematics),
      m_leftSpeed(leftSpeed),
      m_rightSpeed(rightSpeed),
      m_leftController(std::make_unique<frc2::PIDController>(leftController)),
      m_rightController(std::make_unique<frc2::PIDController>(rightController)),
      m_outputVolts(output) {}

RamseteCommand::RamseteCommand(
    frc::Trajectory trajectory, std::function<frc::Pose2d()> pose,
    frc::RamseteController follower,
    frc::DifferentialDriveKinematics kinematics,
    std::function<void(units::meters_per_second_t, units::meters_per_second_t)>
        output)
    : m_trajectory(trajectory),
      m_pose(pose),
      m_follower(follower),
      m_ks(0),
      m_kv(0),
      m_ka(0),
      m_kinematics(kinematics),
      m_outputVel(output) {}

void RamseteCommand::Initialize() {
  m_prevTime = 0_s;
  auto initialState = m_trajectory.Sample(0_s);
  m_prevSpeeds = m_kinematics.ToWheelSpeeds(
      frc::ChassisSpeeds{initialState.velocity, 0_m / 1_s,
                         initialState.velocity * initialState.curvature});
  m_timer.Reset();
  m_timer.Start();
}

void RamseteCommand::Execute() {
  auto curTime = second_t(m_timer.Get());
  auto dt = curTime - m_prevTime;

  auto targetWheelSpeeds = m_kinematics.ToWheelSpeeds(
      m_follower.Calculate(m_pose(), m_trajectory.Sample(curTime)));

  if (m_leftController.get() != nullptr) {
    auto leftFeedforward =
        m_ks * sgn(targetWheelSpeeds.left) + m_kv * targetWheelSpeeds.left +
        m_ka * (targetWheelSpeeds.left - m_prevSpeeds.left) / dt;

    auto rightFeedforward =
        m_ks * sgn(targetWheelSpeeds.right) + m_kv * targetWheelSpeeds.right +
        m_ka * (targetWheelSpeeds.right - m_prevSpeeds.right) / dt;

    auto leftOutput =
        voltage::volt_t(m_leftController->Calculate(
            targetWheelSpeeds.left.to<double>(), m_leftSpeed().to<double>())) +
        leftFeedforward;

    auto rightOutput = voltage::volt_t(m_rightController->Calculate(
                           targetWheelSpeeds.right.to<double>(),
                           m_rightSpeed().to<double>())) +
                       rightFeedforward;

    m_outputVolts(leftOutput, rightOutput);
  } else {
    m_outputVel(targetWheelSpeeds.left, targetWheelSpeeds.right);
  }

  m_prevTime = curTime;
  m_prevSpeeds = targetWheelSpeeds;
}

void RamseteCommand::End(bool interrupted) { m_timer.Stop(); }

bool RamseteCommand::IsFinished() {
  return second_t(m_timer.Get()) - m_trajectory.TotalTime() >= 0_s;
}