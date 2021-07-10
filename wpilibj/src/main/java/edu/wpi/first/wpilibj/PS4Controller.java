// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj;

import edu.wpi.first.hal.FRCNetComm.tResourceType;
import edu.wpi.first.hal.HAL;

/**
 * Handle input from PS4 controllers connected to the Driver Station.
 *
 * <p>This class handles PS4 input that comes from the Driver Station. Each time a value is
 * requested the most recent value is returned. There is a single class instance for each controller
 * and the mapping of ports to hardware buttons depends on the code in the Driver Station.
 */
public class PS4Controller extends GenericHID {
  /**
   * Construct an instance of a device.
   *
   * @param port The port index on the Driver Station that the device is plugged into.
   */
  public PS4Controller(int port) {
    super(port);
    HAL.report(tResourceType.kResourceType_PS4Controller, port + 1);
  }

  /** Represents a digital button on a PS4Controller. */
  public enum Button {
    kSquare(1),
    kCross(2),
    kCircle(3),
    kTriangle(4),
    kL1(5),
    kR1(6),
    kL2(7),
    kR2(8),
    kShare(9),
    kOptions(10),
    kLeftStick(11),
    kRightStick(12),
    kPS(13),
    kScreen(14);

    public final int value;

    Button(int index) {
      this.value = index;
    }
  }

  /** Represents an axis on a PS4Controller. */
  public enum Axis {
    kLeftX(0),
    kLeftY(1),
    kRightX(2),
    kRightY(5),
    kL2(3),
    kR2(4);

    public final int value;

    Axis(int index) {
      value = index;
    }
  }

  /**
   * Get the X axis value of left side of the controller.
   *
   * @return The X axis value of the left controller.
   */
  public double getLeftX() {
    return getRawAxis(Axis.kLeftX.value);
  }

  /**
   * Get the X axis value of right side of the controller.
   *
   * @return The X axis value of the right controller.
   */
  public double getRightX() {
    return getRawAxis(Axis.kRightX.value);
  }

  /**
   * Get the Y axis value of left side of the controller.
   *
   * @return The Y axis value of the left controller.
   */
  public double getLeftY() {
    return getRawAxis(Axis.kLeftY.value);
  }

  /**
   * Get the Y axis value of right side of the controller.
   *
   * @return The Y axis value of the right controller.
   */
  public double getRightY() {
    return getRawAxis(Axis.kRightY.value);
  }

  /**
   * Get the left trigger axis value of the controller.
   *
   * @return The left trigger axis value of the controller.
   */
  public double getL2Axis() {
    return getRawAxis(Axis.kL2.value);
  }

  /**
   * Get the right trigger axis value of the controller.
   *
   * @return The right trigger axis value of the controller.
   */
  public double getR2Axis() {
    return getRawAxis(Axis.kR2.value);
  }

  /**
   * Read the value of the left trigger button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getL2Button() {
    return getRawButton(Button.kL2.value);
  }

  /**
   * Read the value of the right trigger button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getR2Button() {
    return getRawButton(Button.kR2.value);
  }

  /**
   * Whether the left trigger was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getL2ButtonPressed() {
    return getRawButtonPressed(Button.kL2.value);
  }

  /**
   * Whether the right trigger was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getR2ButtonPressed() {
    return getRawButtonPressed(Button.kR2.value);
  }

  /**
   * Whether the left trigger was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getL2ButtonReleased() {
    return getRawButtonReleased(Button.kL2.value);
  }

  /**
   * Whether the right trigger was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getR2ButtonReleased() {
    return getRawButtonPressed(Button.kR2.value);
  }

  /**
   * Read the value of the left bumper button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getL1() {
    return getRawButton(Button.kL1.value);
  }

  /**
   * Read the value of the right bumper button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getR1() {
    return getRawButton(Button.kR1.value);
  }

  /**
   * Whether the left bumper was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getL1Pressed() {
    return getRawButtonPressed(Button.kL1.value);
  }

  /**
   * Whether the right bumper was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getR1Pressed() {
    return getRawButtonPressed(Button.kR1.value);
  }

  /**
   * Whether the left bumper was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getL1Released() {
    return getRawButtonReleased(Button.kL1.value);
  }

  /**
   * Whether the right bumper was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getR1Released() {
    return getRawButtonReleased(Button.kR1.value);
  }

  /**
   * Read the value of the left stick button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getLeftStickButton() {
    return getRawButton(Button.kLeftStick.value);
  }

  /**
   * Read the value of the right stick button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getRightStickButton() {
    return getRawButton(Button.kRightStick.value);
  }

  /**
   * Whether the left stick button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getLeftStickButtonPressed() {
    return getRawButtonPressed(Button.kLeftStick.value);
  }

  /**
   * Whether the right stick button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getRightStickButtonPressed() {
    return getRawButtonPressed(Button.kRightStick.value);
  }

  /**
   * Whether the left stick button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getLeftStickButtonReleased() {
    return getRawButtonReleased(Button.kLeftStick.value);
  }

  /**
   * Whether the right stick button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getRightStickButtonReleased() {
    return getRawButtonReleased(Button.kRightStick.value);
  }

  /**
   * Read the value of the Square button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getSquareButton() {
    return getRawButton(Button.kSquare.value);
  }

  /**
   * Whether the Square button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getSquareButtonPressed() {
    return getRawButtonPressed(Button.kSquare.value);
  }

  /**
   * Whether the Square button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getSquareButtonReleased() {
    return getRawButtonReleased(Button.kSquare.value);
  }

  /**
   * Read the value of the X button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getCrossButton() {
    return getRawButton(Button.kCross.value);
  }

  /**
   * Whether the X button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getCrossButtonPressed() {
    return getRawButtonPressed(Button.kCross.value);
  }

  /**
   * Whether the X button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getCrossButtonReleased() {
    return getRawButtonReleased(Button.kCross.value);
  }

  /**
   * Read the value of the Triangle button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getTriangle() {
    return getRawButton(Button.kTriangle.value);
  }

  /**
   * Whether the Triangle button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getTrianglePressed() {
    return getRawButtonPressed(Button.kTriangle.value);
  }

  /**
   * Whether the Triangle button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getTriangleReleased() {
    return getRawButtonReleased(Button.kTriangle.value);
  }

  /**
   * Read the value of the Circle button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getCircle() {
    return getRawButton(Button.kCircle.value);
  }

  /**
   * Whether the Circle button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getCirclePressed() {
    return getRawButtonPressed(Button.kCircle.value);
  }

  /**
   * Whether the Circle button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getCircleReleased() {
    return getRawButtonReleased(Button.kCircle.value);
  }

  /**
   * Read the value of the share button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getShareButton() {
    return getRawButton(Button.kShare.value);
  }

  /**
   * Whether the share button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getShareButtonPressed() {
    return getRawButtonPressed(Button.kShare.value);
  }

  /**
   * Whether the share button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getShareButtonReleased() {
    return getRawButtonReleased(Button.kShare.value);
  }

  /**
   * Read the value of the PS button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getPSButton() {
    return getRawButton(Button.kPS.value);
  }

  /**
   * Whether the PS button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getPSButtonPressed() {
    return getRawButtonPressed(Button.kPS.value);
  }

  /**
   * Whether the PS button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getPSButtonReleased() {
    return getRawButtonReleased(Button.kPS.value);
  }

  /**
   * Read the value of the options button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getOptionsButton() {
    return getRawButton(Button.kOptions.value);
  }

  /**
   * Whether the options button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getOptionsButtonPressed() {
    return getRawButtonPressed(Button.kOptions.value);
  }

  /**
   * Whether the options button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getOptionsButtonReleased() {
    return getRawButtonReleased(Button.kOptions.value);
  }

  /**
   * Read the value of the screen on the controller.
   *
   * @return The state of the screen.
   */
  public boolean getScreen() {
    return getRawButton(Button.kScreen.value);
  }

  /**
   * Whether the screen was pressed since the last check.
   *
   * @return Whether the screen was pressed since the last check.
   */
  public boolean getScreenPressed() {
    return getRawButtonPressed(Button.kScreen.value);
  }

  /**
   * Whether the screen was released since the last check.
   *
   * @return Whether the screen was released since the last check.
   */
  public boolean getScreenReleased() {
    return getRawButtonReleased(Button.kScreen.value);
  }
}
