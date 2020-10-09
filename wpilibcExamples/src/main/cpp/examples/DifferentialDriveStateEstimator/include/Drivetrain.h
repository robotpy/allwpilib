/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/AnalogGyro.h>
#include <frc/Encoder.h>
#include <frc/PWMVictorSPX.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/controller/PIDController.h>
#include <frc/controller/SimpleMotorFeedforward.h>
#include <frc/estimator/DifferentialDriveStateEstimator.h>
#include <frc/system/plant/LinearSystemId.h>
#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <units/angle.h>
#include <units/angular_velocity.h>
#include <units/length.h>
#include <units/velocity.h>
#include <wpi/math>

/**
 * Represents a differential drive style drivetrain.
 */
class Drivetrain {
 public:
  Drivetrain() {
    m_gyro.Reset();
    // Set the distance per pulse for the drive encoders. We can simply use the
    // distance traveled for one rotation of the wheel divided by the encoder
    // resolution.
    m_leftEncoder.SetDistancePerPulse(2 * wpi::math::pi * kWheelRadius /
                                      kEncoderResolution);
    m_rightEncoder.SetDistancePerPulse(2 * wpi::math::pi * kWheelRadius /
                                       kEncoderResolution);

    m_leftEncoder.Reset();
    m_rightEncoder.Reset();
  }

  static constexpr units::meters_per_second_t kMaxSpeed =
      3.0_mps;  // 3 meters per second
  static constexpr units::radians_per_second_t kMaxAngularSpeed{
      wpi::math::pi};  // 1/2 rotation per second

  void SetSpeeds(const frc::DifferentialDriveWheelSpeeds& speeds);
  void Drive(units::meters_per_second_t xSpeed,
             units::radians_per_second_t rot);
  void UpdateOdometry();

 private:
  static constexpr units::meter_t kTrackWidth = 0.381_m * 2;
  static constexpr double kWheelRadius = 0.0508;  // meters
  static constexpr int kEncoderResolution = 4096;

  frc::PWMVictorSPX m_leftLeader{1};
  frc::PWMVictorSPX m_leftFollower{2};
  frc::PWMVictorSPX m_rightLeader{3};
  frc::PWMVictorSPX m_rightFollower{4};

  frc::SpeedControllerGroup m_leftGroup{m_leftLeader, m_leftFollower};
  frc::SpeedControllerGroup m_rightGroup{m_rightLeader, m_rightFollower};

  frc::Encoder m_leftEncoder{0, 1};
  frc::Encoder m_rightEncoder{2, 3};

  frc2::PIDController m_leftPIDController{1.0, 0.0, 0.0};
  frc2::PIDController m_rightPIDController{1.0, 0.0, 0.0};

  frc::AnalogGyro m_gyro{0};

  frc::DifferentialDriveKinematics m_kinematics{kTrackWidth};

  // Gains are for example purposes only - must be determined for your own
  // robot!
  frc::LinearSystem<2, 2, 2> m_drivetrainSystem = frc::LinearSystemId::IdentifyDrivetrainSystem(3, 4, 1, 2);

  // Gains are for example purposes only - must be determined for your own
  // robot!
  frc::DifferentialDriveStateEstimator m_stateEstimator{
      m_drivetrainSystem,
      Eigen::Matrix<double, 10, 1>::Zero(),
      frc::MakeMatrix<10, 1>(0.002, 0.002, 0.0001, 1.5, 1.5, 0.5, 0.5, 10.0,
                             10.0, 2.0),
      frc::MakeMatrix<3, 1>(0.0001, 0.005, 0.005),
      frc::MakeMatrix<3, 1>(0.1, 0.1, 0.1),
      m_kinematics,
      20_ms};

  // Gains are for example purposes only - must be determined for your own
  // robot!
  frc::SimpleMotorFeedforward<units::meters> m_feedforward{1_V, 3_V / 1_mps};
};
