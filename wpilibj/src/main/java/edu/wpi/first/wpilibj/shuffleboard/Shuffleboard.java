/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.shuffleboard;

import edu.wpi.first.networktables.NetworkTableInstance;

/**
 * The Shuffleboard class provides a mechanism with which data can be added and laid out in the
 * Shuffleboard dashboard application from a robot program. Tabs and layouts can be specified, as
 * well as choosing which widgets to display with and setting properties of these widgets; for
 * example, programmers can specify a specific {@code boolean} value to be displayed with a toggle
 * button instead of the default colored box, or set custom colors for that box.
 *
 * <p>For example, displaying a boolean entry with a toggle button:
 * <pre>{@code
 * NetworkTableEntry myBoolean = Shuffleboard.getTab("Example Tab")
 *   .add("My Boolean", false)
 *   .withWidget("Toggle Button")
 *   .getEntry();
 * }</pre>
 *
 * Changing the colors of the boolean box:
 * <pre>{@code
 * NetworkTableEntry myBoolean = Shuffleboard.getTab("Example Tab")
 *   .add("My Boolean", false)
 *   .withWidget("Boolean Box")
 *   .withProperties(Map.of("colorWhenTrue", "green", "colorWhenFalse", "maroon"))
 *   .getEntry();
 * }</pre>
 *
 * Specifying a parent layout. Note that the layout type must <i>always</i> be specified, even if
 * the layout has already been generated by a previously defined entry.
 * <pre>{@code
 * NetworkTableEntry myBoolean = Shuffleboard.getTab("Example Tab")
 *   .getLayout("List", "Example List")
 *   .add("My Boolean", false)
 *   .withWidget("Toggle Button")
 *   .getEntry();
 * }</pre>
 * </p>
 *
 * <p>Teams are encouraged to set up shuffleboard layouts at the start of the robot program.</p>
 */
public final class Shuffleboard {
  /**
   * The name of the base NetworkTable into which all Shuffleboard data will be added.
   */
  public static final String BASE_TABLE_NAME = "/Shuffleboard";

  private static final ShuffleboardRoot root =
      new ShuffleboardInstance(NetworkTableInstance.getDefault());

  // TODO usage reporting

  private Shuffleboard() {
    throw new UnsupportedOperationException("This is a utility class and cannot be instantiated");
  }

  /**
   * Updates all the values in Shuffleboard. Iterative and timed robots are pre-configured to call
   * this method in the main robot loop; teams using custom robot base classes, or subclass
   * SampleRobot, should make sure to call this repeatedly to keep data on the dashboard up to date.
   */
  public static void update() {
    root.update();
  }

  /**
   * Gets the Shuffleboard tab with the given title, creating it if it does not already exist.
   *
   * @param title the title of the tab
   * @return the tab with the given title
   */
  public static ShuffleboardTab getTab(String title) {
    return root.getTab(title);
  }

  /**
   * Enables user control of widgets containing actuators: speed controllers, relays, etc. This
   * should only be used when the robot is in test mode. IterativeRobotBase and SampleRobot are
   * both configured to call this method when entering test mode; most users should not need to use
   * this method directly.
   */
  public static void enableActuatorWidgets() {
    root.enableActuatorWidgets();
  }

  /**
   * Disables user control of widgets containing actuators. For safety reasons, actuators should
   * only be controlled while in test mode. IterativeRobotBase and SampleRobot are both configured
   * to call this method when exiting in test mode; most users should not need to use
   * this method directly.
   */
  public static void disableActuatorWidgets() {
    update(); // Need to update to make sure the sendable builders are initialized
    root.disableActuatorWidgets();
  }

}
