/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <functional>

#include <units/units.h>

#include "CommandBase.h"
#include "CommandHelper.h"
#include "frc/Timer.h"
#include "frc/controller/PIDController.h"
#include "frc/controller/RamseteController.h"
#include "frc/geometry/Pose2d.h"
#include "frc/kinematics/DifferentialDriveKinematics.h"
#include "frc/trajectory/Trajectory.h"

#pragma once

namespace frc2 {
class RamseteCommand : public CommandHelper<CommandBase, RamseteCommand> {
  using voltsecondspermeter =
      units::compound_unit<units::voltage::volt, units::second,
                           units::inverse<units::meter>>;
  using voltsecondssquaredpermeter =
      units::compound_unit<units::voltage::volt, units::squared<units::second>,
                           units::inverse<units::meter>>;

 public:
  RamseteCommand(
      frc::Trajectory trajectory, std::function<frc::Pose2d()> pose,
      frc::RamseteController follower, units::voltage::volt_t ks,
      units::unit_t<voltsecondspermeter> kv,
      units::unit_t<voltsecondssquaredpermeter> ka,
      frc::DifferentialDriveKinematics kinematics,
      std::function<units::meters_per_second_t()> leftSpeed,
      std::function<units::meters_per_second_t()> rightSpeed,
      frc2::PIDController leftController, frc2::PIDController rightController,
      std::function<void(units::voltage::volt_t, units::voltage::volt_t)>
          output);

  void Initialize() override;

  void Execute() override;

  void End(bool interrupted) override;

  bool IsFinished() override;

 private:
  frc::Trajectory m_trajectory;
  std::function<frc::Pose2d()> m_pose;
  frc::RamseteController m_follower;
  const units::voltage::volt_t m_ks;
  const units::unit_t<voltsecondspermeter> m_kv;
  const units::unit_t<voltsecondssquaredpermeter> m_ka;
  frc::DifferentialDriveKinematics m_kinematics;
  std::function<units::meters_per_second_t()> m_leftSpeed;
  std::function<units::meters_per_second_t()> m_rightSpeed;
  frc2::PIDController m_leftController;
  frc2::PIDController m_rightController;
  std::function<void(units::voltage::volt_t, units::voltage::volt_t)> m_output;

  frc::Timer m_timer;
  units::second_t m_prevTime;
  frc::DifferentialDriveWheelSpeeds m_prevSpeeds;
};
}  // namespace frc2
