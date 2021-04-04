// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.networktables;

/** NetworkTables publish/subscribe option. */
public class PubSubOption {
  private static final int kTypeImmediate = 0;
  private static final int kTypePeriodic = 1;
  private static final int kTypeLogging = 2;

  protected PubSubOption(int type, double value) {
    this.type = type;
    this.value = value;
  }

  /**
   * If specified, value updates should be immediately transmitted over the network, rather than
   * waiting for a periodic "sweep" of changed values. Note this will increase network
   * bandwidth utilization; use with caution! This option has no effect if NetworkTables
   * is running in server mode.
   */
  public static PubSubOption kImmediate = new PubSubOption(kTypeImmediate, 1);

  /**
   * How frequently changes should be sent over the network. NetworkTables may send
   * more frequently than this (e.g. use a combined minimum period for all values)
   * or apply a restricted range to this value. The default if unspecified
   * (and the immediate flag is false) is 100 ms. This option and the immediate option
   * override each other.
   */
  public static PubSubOption periodic(double time) {
    return new PubSubOption(kTypePeriodic, time);
  }

  /**
   * Send all value changes over the network. This option defaults to enabled.
   */
  public static PubSubOption kLogging = new PubSubOption(kTypeLogging, 1);

  /**
   * Only send the most recent value over the network when a periodic sweep occurs.
   */
  public static PubSubOption kDisableLogging = new PubSubOption(kTypeLogging, 0);

  final int type;
  final double value;
}
