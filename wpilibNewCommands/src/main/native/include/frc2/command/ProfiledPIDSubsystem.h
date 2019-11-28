/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/controller/ProfiledPIDController.h>
#include <units/units.h>

#include "frc2/command/SubsystemBase.h"

namespace frc2 {
/**
 * A subsystem that uses a ProfiledPIDController to control an output.  The
 * controller is run synchronously from the subsystem's periodic() method.
 *
 * @see ProfiledPIDController
 */
template <class Distance>
class ProfiledPIDSubsystem : public SubsystemBase {
  using Distance_t = units::unit_t<Distance>;
  using Velocity =
      units::compound_unit<Distance, units::inverse<units::seconds>>;
  using Velocity_t = units::unit_t<Velocity>;
  using State = typename frc::TrapezoidProfile<Distance>::State;

 public:
  /**
   * Creates a new ProfiledPIDSubsystem.
   *
   * @param controller the ProfiledPIDController to use
   */
  explicit ProfiledPIDSubsystem(frc::ProfiledPIDController<Distance> controller)
      : m_controller{controller} {}

  void Periodic() override {
    if (m_enabled) {
      UseOutput(m_controller.Calculate(GetMeasurement(), m_goal),
                m_controller.GetSetpoint());
    }
  }

  /**
   * Sets the goal state for the subsystem.
   *
   * @param goal The goal state for the subsystem's motion profile.
   */
  void SetGoal(State goal) { m_goal = goal; }

  /**
   * Sets the goal state for the subsystem.  Goal velocity assumed to be zero.
   *
   * @param goal The goal position for the subsystem's motion profile.
   */
  void SetGoal(Distance_t goal) { m_goal = State{goal, Velocity_t(0)}; }

  /**
   * Enables the PID control.  Resets the controller.
   */
  virtual void Enable() {
    m_controller.Reset();
    m_enabled = true;
  }

  /**
   * Disables the PID control.  Sets output to zero.
   */
  virtual void Disable() {
    UseOutput(0, State{Distance_t(0), Velocity_t(0)});
    m_enabled = false;
  }

  /**
   * Returns whether the controller is enabled.
   *
   * @return Whether the controller is enabled.
   */
  bool IsEnabled() { return m_enabled; }

  /**
   * Returns the ProfiledPIDController.
   *
   * @return The controller.
   */
  frc::ProfiledPIDController<Distance>& GetController() { return m_controller; }

 protected:
  frc::ProfiledPIDController<Distance> m_controller;
  bool m_enabled{false};

  /**
   * Returns the measurement of the process variable used by the
   * ProfiledPIDController.
   *
   * @return the measurement of the process variable
   */
  virtual Distance_t GetMeasurement() = 0;

  /**
   * Uses the output from the ProfiledPIDController.
   *
   * @param output the output of the ProfiledPIDController
   * @param setpoint the setpoint state of the ProfiledPIDController, for
   * feedforward
   */
  virtual void UseOutput(double output, State setpoint) = 0;

 private:
  State m_goal;
};
}  // namespace frc2
