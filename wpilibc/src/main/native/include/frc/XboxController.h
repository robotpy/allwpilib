// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "frc/GenericHID.h"

namespace frc {

/**
 * Handle input from Xbox 360 or Xbox One controllers connected to the Driver
 * Station.
 *
 * This class handles Xbox input that comes from the Driver Station. Each time a
 * value is requested the most recent value is returned. There is a single class
 * instance for each controller and the mapping of ports to hardware buttons
 * depends on the code in the Driver Station.
 */
class XboxController : public GenericHID {
 public:
  /**
   * Construct an instance of an Xbox controller.
   *
   * The controller index is the USB port on the Driver Station.
   *
   * @param port The port on the Driver Station that the controller is plugged
   *             into (0-5).
   */
  explicit XboxController(int port);

  ~XboxController() override = default;

  XboxController(XboxController&&) = default;
  XboxController& operator=(XboxController&&) = default;

  /**
   * Get the left X axis value of the controller.
   */
  double GetLeftX() const;

  /**
   * Get the right X axis value of the controller.
   */
  double GetRightX() const;

  /**
   * Get the left Y axis value of the controller.
   */
  double GetLeftY() const;

  /**
   * Get the right Y axis value of the controller.
   */
  double GetRightY() const;

  /**
   * Get the left trigger axis axis value of the controller.
   */
  double GetLeftTriggerAxis() const;

  /**
   * Get the right TriggerAxis axis value of the controller.
   */
  double GetRightTriggerAxis() const;

  /**
   * Read the value of the left bumper button on the controller.
   */
  bool GetLeftBumper() const;

  /**
   * Read the value of the right bumper button on the controller.
   */
  bool GetRightBumper() const;

  /**
   * Whether the left bumper was pressed since the last check.
   */
  bool GetLeftBumperPressed();

  /**
   * Whether the right bumper was pressed since the last check.
   */
  bool GetRightBumperPressed();

  /**
   * Whether the left bumper was released since the last check.
   */
  bool GetLeftBumperReleased();

  /**
   * Whether the right bumper was released since the last check.
   */
  bool GetRightBumperReleased();

  /**
   * Read the value of the left stick button on the controller.
   */
  bool GetLeftStickButton() const;

  /**
   * Read the value of the right stick button on the controller.
   */
  bool GetRightStickButton() const;

  /**
   * Whether the left stick button was pressed since the last check.
   */
  bool GetLeftStickButtonPressed();

  /**
   * Whether the right stick button was pressed since the last check.
   */
  bool GetRightStickButtonPressed();

  /**
   * Whether the left stick button was released since the last check.
   */
  bool GetLeftStickButtonReleased();

  /**
   * Whether the right stick button was released since the last check.
   */
  bool GetRightStickButtonReleased();

  /**
   * Read the value of the A button on the controller.
   *
   * @return The state of the button.
   */
  bool GetAButton() const;

  /**
   * Whether the A button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  bool GetAButtonPressed();

  /**
   * Whether the A button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  bool GetAButtonReleased();

  /**
   * Read the value of the B button on the controller.
   *
   * @return The state of the button.
   */
  bool GetBButton() const;

  /**
   * Whether the B button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  bool GetBButtonPressed();

  /**
   * Whether the B button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  bool GetBButtonReleased();

  /**
   * Read the value of the X button on the controller.
   *
   * @return The state of the button.
   */
  bool GetXButton() const;

  /**
   * Whether the X button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  bool GetXButtonPressed();

  /**
   * Whether the X button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  bool GetXButtonReleased();

  /**
   * Read the value of the Y button on the controller.
   *
   * @return The state of the button.
   */
  bool GetYButton() const;

  /**
   * Whether the Y button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  bool GetYButtonPressed();

  /**
   * Whether the Y button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  bool GetYButtonReleased();

  /**
   * Whether the Y button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  bool GetBackButton() const;

  /**
   * Whether the back button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  bool GetBackButtonPressed();

  /**
   * Whether the back button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  bool GetBackButtonReleased();

  /**
   * Read the value of the start button on the controller.
   *
   * @return The state of the button.
   */
  bool GetStartButton() const;

  /**
   * Whether the start button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  bool GetStartButtonPressed();

  /**
   * Whether the start button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  bool GetStartButtonReleased();

  enum class Button {
    kBumperLeft = 5,
    kBumperRight = 6,
    kStickLeft = 9,
    kStickRight = 10,
    kA = 1,
    kB = 2,
    kX = 3,
    kY = 4,
    kBack = 7,
    kStart = 8
  };

  enum class Axis {
    kLeftX = 0,
    kRightX = 4,
    kLeftY = 1,
    kRightY = 5,
    kLeftTrigger = 2,
    kRightTrigger = 3
  };
};

}  // namespace frc
