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
    kX(2),
    kCircle(3),
    kTriangle(4),
    kLeftBumper(5),
    kRightBumper(6),
    kLeftTrigger(7),
    kRightTrigger(8),
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
    kLeftTrigger(3),
    kRightTrigger(4);

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
  public double getLeftTriggerAxis() {
    return getRawAxis(Axis.kLeftTrigger.value);
  }

  /**
   * Get the right trigger axis value of the controller.
   *
   * @return The right trigger axis value of the controller.
   */
  public double getRightTriggerAxis() {
    return getRawAxis(Axis.kRightTrigger.value);
  }

  /**
   * Read the value of the left trigger button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getLeftTriggerButton() {
    return getRawButton(Button.kLeftTrigger.value);
  }

  /**
   * Read the value of the right trigger button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getRightTriggerButton() {
    return getRawButton(Button.kRightTrigger.value);
  }

  /**
   * Whether the left trigger was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getLeftTriggerPressedButton() {
    return getRawButtonPressed(Button.kLeftTrigger.value);
  }

  /**
   * Whether the right trigger was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getRightTriggerButtonPressed() {
    return getRawButtonPressed(Button.kRightTrigger.value);
  }

  /**
   * Whether the left trigger was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getLeftTriggerButtonReleased() {
    return getRawButtonReleased(Button.kLeftTrigger.value);
  }

  /**
   * Whether the right trigger was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getRightTriggerButtonReleased() {
    return getRawButtonPressed(Button.kRightTrigger.value);
  }

  /**
   * Read the value of the left bumper button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getLeftBumper() {
    return getRawButton(Button.kLeftBumper.value);
  }

  /**
   * Read the value of the right bumper button on the controller.
   *
   * @return The state of the button.
   */
  public boolean getRightBumper() {
    return getRawButton(Button.kRightBumper.value);
  }

  /**
   * Whether the left bumper was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getLeftBumperPressed() {
    return getRawButtonPressed(Button.kLeftBumper.value);
  }

  /**
   * Whether the right bumper was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getRightBumperPressed() {
    return getRawButtonPressed(Button.kRightBumper.value);
  }

  /**
   * Whether the left bumper was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getLeftBumperReleased() {
    return getRawButtonReleased(Button.kLeftBumper.value);
  }

  /**
   * Whether the right bumper was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getRightBumperReleased() {
    return getRawButtonPressed(Button.kRightBumper.value);
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
  public boolean getXButton() {
    return getRawButton(Button.kX.value);
  }

  /**
   * Whether the X button was pressed since the last check.
   *
   * @return Whether the button was pressed since the last check.
   */
  public boolean getXButtonPressed() {
    return getRawButtonPressed(Button.kX.value);
  }

  /**
   * Whether the X button was released since the last check.
   *
   * @return Whether the button was released since the last check.
   */
  public boolean getXButtonReleased() {
    return getRawButtonReleased(Button.kX.value);
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
