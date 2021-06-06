// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "frc/simulation/GenericHIDSim.h"

namespace frc {

class XboxController;

namespace sim {

/**
 * Class to control a simulated Xbox 360 or Xbox One controller.
 */
class XboxControllerSim : public GenericHIDSim {
 public:
  /**
   * Constructs from a XboxController object.
   *
   * @param joystick controller to simulate
   */
  explicit XboxControllerSim(const XboxController& joystick);

  /**
   * Constructs from a joystick port number.
   *
   * @param port port number
   */
  explicit XboxControllerSim(int port);

  /**
   * Change the left X value of the joystick.
   *
   * @param value the new value
   */
  void SetLeftX(double value);

  /**
   * Change the right X value of the joystick.
   *
   * @param value the new value
   */
  void SetRightX(double value);

  /**
   * Change the left Y value of the joystick.
   *
   * @param value the new value
   */
  void SetLeftY(double value);

  /**
   * Change the right Y value of the joystick.
   *
   * @param value the new value
   */
  void SetRightY(double value);

  /**
   * Change the left trigger axis value of the joystick.
   *
   * @param value the new value
   */
  void SetLeftTriggerAxis(double value);

  /**
   * Change the right trigger axis value of the joystick.
   *
   * @param value the new value
   */
  void SetRightTriggerAxis(double value);

  /**
   * Change the left bumper value of the joystick.
   *
   * @param value the new value
   */
  void SetLeftBumper(bool state);

  /**
   * Change the right bumper value of the joystick.
   *
   * @param value the new value
   */
  void SetRightBumper(bool state);

  /**
   * Change the left button value of the joystick.
   *
   * @param value the new value
   */
  void SetLeftStickButton(bool state);

  /**
   * Change the right button value of the joystick.
   *
   * @param value the new value
   */
  void SetRightStickButton(bool state);

  /**
   * Change the value of the A button.
   *
   * @param state the new value
   */
  void SetAButton(bool state);

  /**
   * Change the value of the B button.
   *
   * @param state the new value
   */
  void SetBButton(bool state);

  /**
   * Change the value of the X button.
   *
   * @param state the new value
   */
  void SetXButton(bool state);

  /**
   * Change the value of the Y button.
   *
   * @param state the new value
   */
  void SetYButton(bool state);

  /**
   * Change the value of the Back button.
   *
   * @param state the new value
   */
  void SetBackButton(bool state);

  /**
   * Change the value of the Start button.
   *
   * @param state the new value
   */
  void SetStartButton(bool state);
};

}  // namespace sim
}  // namespace frc
