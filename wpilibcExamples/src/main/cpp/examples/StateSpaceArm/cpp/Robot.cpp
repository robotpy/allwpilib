/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <frc/Encoder.h>
#include <frc/GenericHID.h>
#include <frc/PWMVictorSPX.h>
#include <frc/StateSpaceUtil.h>
#include <frc/TimedRobot.h>
#include <frc/XboxController.h>
#include <frc/controller/LinearPlantInversionFeedforward.h>
#include <frc/controller/LinearQuadraticRegulator.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/estimator/KalmanFilter.h>
#include <frc/system/LinearSystemLoop.h>
#include <frc/system/plant/DCMotor.h>
#include <frc/system/plant/LinearSystemId.h>
#include <frc/trajectory/TrapezoidProfile.h>
#include <units/angle.h>
#include <units/moment_of_inertia.h>
#include <wpi/math>

/**
 * This is a sample program to demonstrate how to use a state-space controller
 * to control an arm.
 */
class Robot : public frc::TimedRobot {
  static constexpr int kMotorPort = 0;
  static constexpr int kEncoderAChannel = 0;
  static constexpr int kEncoderBChannel = 1;
  static constexpr int kJoystickPort = 0;

  static constexpr units::radian_t kRaisedPosition = 90_deg;
  static constexpr units::radian_t kLoweredPosition = 0_deg;

  // Moment of inertia of the arm. Can be estimated with CAD. If finding this
  // constant is difficult, LinearSystem.identifyPositionSystem may be better.
  static constexpr units::kilogram_square_meter_t kArmMOI = 1.2_kg_sq_m;

  // Reduction between motors and encoder, as output over input. If the arm
  // spins slower than the motors, this number should be greater than one.
  static constexpr double kArmGearing = 10.0;

  // The plant holds a state-space model of our arm. This system has the
  // following properties:
  //
  // States: [position, velocity], in radians and radians per second.
  // Inputs (what we can "put in"): [voltage], in volts.
  // Outputs (what we can measure): [position], in radians.
  frc::LinearSystem<2, 1, 1> m_armPlant =
      frc::LinearSystemId::SingleJointedArmSystem(frc::DCMotor::NEO(2), kArmMOI,
                                                  kArmGearing);

  // The observer fuses our encoder data and voltage inputs to reject noise.
  frc::KalmanFilter<2, 1, 1> m_observer{
      m_armPlant,
      {0.015, 0.17},  // How accurate we think our model is
      {0.01},         // How accurate we think our encoder position
      // data is. In this case we very highly trust our encoder position
      // reading.
      20_ms};

  // A LQR uses feedback to create voltage commands.
  frc::LinearQuadraticRegulator<2, 1> m_controller{
      m_armPlant,
      // qelms. Velocity error tolerance, in radians and radians per second.
      // Decrease this to more heavily penalize state excursion, or make the
      // controller behave more aggressively.
      {1.0 * 2.0 * wpi::math::pi / 360.0, 10.0 * 2.0 * wpi::math::pi / 360.0},
      // relms. Control effort (voltage) tolerance. Decrease this to more
      // heavily penalize control effort, or make the controller less
      // aggressive. 12 is a good starting point because that is the
      // (approximate) maximum voltage of a battery.
      {12.0},
      // Nominal time between loops. 20ms for TimedRobot, but can be lower if
      // using notifiers.
      20_ms};

  // The state-space loop combines a controller, observer, feedforward and plant
  // for easy control.
  frc::LinearSystemLoop<2, 1, 1> m_loop{m_armPlant, m_controller, m_observer,
                                        12_V, 20_ms};

  // An encoder set up to measure arm position in radians per second.
  frc::Encoder m_encoder{kEncoderAChannel, kEncoderBChannel};

  frc::PWMVictorSPX m_motor{kMotorPort};
  frc::XboxController m_joystick{kJoystickPort};

  frc::TrapezoidProfile<units::radians>::Constraints m_constraints{
      45_deg_per_s, 90_deg_per_s / 1_s};

  frc::TrapezoidProfile<units::radians>::State m_lastProfiledReference;

 public:
  void RobotInit() {
    // We go 2 pi radians per 4096 clicks.
    m_encoder.SetDistancePerPulse(2.0 * wpi::math::pi / 4096.0);
  }

  void TeleopInit() {
    m_loop.Reset(
        frc::MakeMatrix<2, 1>(m_encoder.GetDistance(), m_encoder.GetRate()));

    m_lastProfiledReference = {
        units::radian_t(m_encoder.GetDistance()),
        units::radians_per_second_t(m_encoder.GetRate())};
  }

  void TeleopPeriodic() {
    // Sets the target position of our arm. This is similar to setting the
    // setpoint of a PID controller.
    frc::TrapezoidProfile<units::radians>::State goal;
    if (m_joystick.GetBumper(frc::GenericHID::kRightHand)) {
      // We pressed the bumper, so let's set our next reference
      goal = {kRaisedPosition, 0_rad_per_s};
    } else {
      // We released the bumper, so let's spin down
      goal = {kLoweredPosition, 0_rad_per_s};
    }
    m_lastProfiledReference =
        (frc::TrapezoidProfile<units::radians>(m_constraints, goal,
                                               m_lastProfiledReference))
            .Calculate(20_ms);

    m_loop.SetNextR(
        frc::MakeMatrix<2, 1>(m_lastProfiledReference.position.to<double>(),
                              m_lastProfiledReference.velocity.to<double>()));

    // Correct our Kalman filter's state vector estimate with encoder data.
    m_loop.Correct(frc::MakeMatrix<1, 1>(m_encoder.GetDistance()));

    // Update our LQR to generate new voltage commands and use the voltages to
    // predict the next state with out Kalman filter.
    m_loop.Predict(20_ms);

    // Send the new calculated voltage to the motors.
    // voltage = duty cycle * battery voltage, so
    // duty cycle = voltage / battery voltage
    m_motor.SetVoltage(units::volt_t(m_loop.U(0)));
  }
};

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
