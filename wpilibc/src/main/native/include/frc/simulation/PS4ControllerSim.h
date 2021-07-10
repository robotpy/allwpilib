// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "frc/simulation/GenericHIDSim.h"

namespace frc {

class PS4Controller;

namespace sim {

/**
 * Class to control a simulated PS4 controller.
 */
class PS4ControllerSim : public GenericHIDSim {
 public:
  /**
   *ructs from a PS4Controller object.
   *
   * @param joystick controller to simulate
   */
  explicit PS4ControllerSim(const PS4Controller& joystick);

  /**
   *ructs from a joystick port number.
   *
   * @param port port number
   */
  explicit PS4ControllerSim(int port);

  /**
   * Change the left X axis value of the controller.
   */
  void SetLeftX(double value);

  /**
   * Change the right X axis value of the controller.
   */
  void SetRightX(double value);

  /**
   * Change the left Y axis value of the controller.
   */
  void SetLeftY(double value);

  /**
   * Change the right Y axis value of the controller.
   */
  void SetRightY(double value);

  /**
   * Change the L2 axis axis value of the controller.
   */
  void SetL2Axis(double value);

  /**
   * Change the R2 axis value of the controller.
   */
  void SetR2Axis(double value);

  /**
   * Change the value of the Square button on the controller.
   *
   * @param value the new value
   */
  void SetSquareButton(bool value);

  /**
   * Change the value of the Cross button on the controller.
   *
   * @param value the new value
   */
  void SetCrossButton(bool value);

  /**
   * Change the value of the Circle button on the controller.
   *
   * @param value the new value
   */
  void SetCircleButton(bool value);

  /**
   * Change the value of the Triangle button on the controller.
   *
   * @param value the new value
   */
  void SetTriangleButton(bool value);

  /**
   * Change the value of the L1 button on the controller.
   *
   * @param value the new value
   */
  void SetL1Button(bool value);

  /**
   * Change the value of the R1 button on the controller.
   *
   * @param value the new value
   */
  void SetR1Button(bool value);

  /**
   * Change the value of the L2 button on the controller.
   *
   * @param value the new value
   */
  void SetL2Button(bool value);

  /**
   * Change the value of the R2 button on the controller.
   *
   * @param value the new value
   */
  void SetR2Button(bool value);

  /**
   * Change the value of the Share button on the controller.
   *
   * @param value the new value
   */
  void SetShareButton(bool value);

  /**
   * Change the value of the Options button on the controller.
   *
   * @param value the new value
   */
  void SetOptionsButton(bool value);

  /**
   * Change the value of the left stick button on the controller.
   *
   * @param value the new value
   */
  void SetLeftStickButton(bool value);

  /**
   * Change the value of the right stick button on the controller.
   *
   * @param value the new value
   */
  void SetRightStickButton(bool value);

  /**
   * Change the value of the PS button on the controller.
   *
   * @param value the new value
   */
  void SetPSButton(bool value);

  /**
   * Change the value of the screen button on the controller.
   *
   * @param value the new value
   */
  void SetScreen(bool value);
};

}  // namespace sim
}  // namespace frc
