/*----------------------------------------------------------------------------*/
/* Use and modify this template how you wish, it's public domain.             */
/*----------------------------------------------------------------------------*/

#pragma once

#include <AnalogGyro.h>
#include <Commands/Subsystem.h>
#include <Drive/DifferentialDrive.h>
#include <Encoder.h>
#include <Spark.h>
#include <SpeedControllerGroup.h>

namespace frc {
class Joystick;
}  // namespace frc

/**
 * The DriveTrain subsystem controls the robot's chassis and reads in
 * information about it's speed and position.
 */
class DriveTrain : public frc::Subsystem {
 public:
  DriveTrain();

  /**
   * When other commands aren't using the drivetrain, allow tank drive
   * with
   * the joystick.
   */
  void InitDefaultCommand();

  /**
   * @param leftAxis Left sides value
   * @param rightAxis Right sides value
   */
  void TankDrive(double leftAxis, double rightAxis);

  /**
   * Stop the drivetrain from moving.
   */
  void Stop();

  /**
   * @return The encoder getting the distance and speed of left side of
   * the drivetrain.
   */
  frc::Encoder& GetLeftEncoder();

  /**
   * @return The encoder getting the distance and speed of right side of
   * the drivetrain.
   */
  frc::Encoder& GetRightEncoder();

  /**
   * @return The current angle of the drivetrain.
   */
  double GetAngle();

 private:
  // Subsystem devices
  frc::Spark m_frontLeftCIM{1};
  frc::Spark m_rearLeftCIM{2};
  frc::SpeedControllerGroup m_leftCIMs{m_frontLeftCIM, m_rearLeftCIM};

  frc::Spark m_frontRightCIM{3};
  frc::Spark m_rearRightCIM{4};
  frc::SpeedControllerGroup m_rightCIMs{m_frontRightCIM, m_rearRightCIM};

  frc::DifferentialDrive m_robotDrive{m_leftCIMs, m_rightCIMs};

  frc::Encoder m_rightEncoder{1, 2, true, frc::Encoder::k4X};
  frc::Encoder m_leftEncoder{3, 4, false, frc::Encoder::k4X};
  frc::AnalogGyro m_gyro{0};
};
