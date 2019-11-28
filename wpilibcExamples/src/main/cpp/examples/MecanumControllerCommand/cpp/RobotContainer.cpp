/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "RobotContainer.h"

#include <frc/controller/PIDController.h>
#include <frc/geometry/Translation2d.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/trajectory/Trajectory.h>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc/trajectory/constraint/MecanumDriveKinematicsConstraint.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/MecanumControllerCommand.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/button/JoystickButton.h>

#include "Constants.h"

using namespace DriveConstants;

const frc::MecanumDriveKinematics DriveConstants::kDriveKinematics{
    frc::Translation2d(kWheelBase / 2, kTrackWidth / 2),
    frc::Translation2d(kWheelBase / 2, -kTrackWidth / 2),
    frc::Translation2d(-kWheelBase / 2, kTrackWidth / 2),
    frc::Translation2d(-kWheelBase / 2, -kTrackWidth / 2)};

RobotContainer::RobotContainer() {
  // Initialize all of your commands and subsystems here

  // Configure the button bindings
  ConfigureButtonBindings();

  // Set up default drive command
  m_drive.SetDefaultCommand(frc2::RunCommand(
      [this] {
        m_drive.Drive(m_driverController.GetY(frc::GenericHID::kLeftHand),
                      m_driverController.GetX(frc::GenericHID::kRightHand),
                      m_driverController.GetX(frc::GenericHID::kLeftHand),
                      false);
      },
      {&m_drive}));
}

void RobotContainer::ConfigureButtonBindings() {
  // Configure your button bindings here

  // While holding the shoulder button, drive at half speed
  frc2::JoystickButton(&m_driverController, 6)
      .WhenPressed(&m_driveHalfSpeed)
      .WhenReleased(&m_driveFullSpeed);
}

frc2::Command* RobotContainer::GetAutonomousCommand() {
  // Set up config for trajectory
  frc::TrajectoryConfig config(AutoConstants::kMaxSpeed,
                               AutoConstants::kMaxAcceleration);
  // Add kinematics to ensure max speed is actually obeyed
  config.SetKinematics(DriveConstants::kDriveKinematics);

  // An example trajectory to follow.  All units in meters.
  auto exampleTrajectory = frc::TrajectoryGenerator::GenerateTrajectory(
      // Start at the origin facing the +X direction
      frc::Pose2d(0_m, 0_m, frc::Rotation2d(0_deg)),
      // Pass through these two interior waypoints, making an 's' curve path
      {frc::Translation2d(1_m, 1_m), frc::Translation2d(2_m, -1_m)},
      // End 3 meters straight ahead of where we started, facing forward
      frc::Pose2d(3_m, 0_m, frc::Rotation2d(0_deg)),
      // Pass the config
      config);

  frc2::MecanumControllerCommand mecanumControllerCommand(
      exampleTrajectory, [this]() { return m_drive.GetPose(); },

      frc::SimpleMotorFeedforward<units::meters>(ks, kv, ka),
      DriveConstants::kDriveKinematics,

      frc2::PIDController(AutoConstants::kPXController, 0, 0),
      frc2::PIDController(AutoConstants::kPYController, 0, 0),
      frc::ProfiledPIDController<units::radians>(
          AutoConstants::kPThetaController, 0, 0,
          AutoConstants::kThetaControllerConstraints),

      AutoConstants::kMaxSpeed,

      [this]() {
        return frc::MecanumDriveWheelSpeeds{
            units::meters_per_second_t(m_drive.GetFrontLeftEncoder().GetRate()),
            units::meters_per_second_t(
                m_drive.GetFrontRightEncoder().GetRate()),
            units::meters_per_second_t(m_drive.GetRearLeftEncoder().GetRate()),
            units::meters_per_second_t(
                m_drive.GetRearRightEncoder().GetRate())};
      },

      frc2::PIDController(DriveConstants::kPFrontLeftVel, 0, 0),
      frc2::PIDController(DriveConstants::kPRearLeftVel, 0, 0),
      frc2::PIDController(DriveConstants::kPFrontRightVel, 0, 0),
      frc2::PIDController(DriveConstants::kPRearRightVel, 0, 0),

      [this](units::volt_t frontLeft, units::volt_t rearLeft,
             units::volt_t frontRight, units::volt_t rearRight) {
        m_drive.SetSpeedControllersVolts(frontLeft, rearLeft, frontRight,
                                         rearRight);
      },

      {&m_drive});

  // no auto
  return new frc2::SequentialCommandGroup(
      std::move(mecanumControllerCommand),
      frc2::InstantCommand([this]() { m_drive.Drive(0, 0, 0, false); }, {}));
}
