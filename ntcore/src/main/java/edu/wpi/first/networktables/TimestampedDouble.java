// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.networktables;

/** NetworkTables timestamped double. */
public class TimestampedDouble {
  public TimestampedDouble(double value, long timestamp) {
    this.value = value;
    this.timestamp = timestamp;
  }

  public final double value;
  public final long timestamp;
}
