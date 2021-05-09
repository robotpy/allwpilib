// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.networktables;

import java.util.function.DoubleSupplier;

/** NetworkTables double subscriber. */
public interface DoubleSubscriber extends Subscriber, DoubleSupplier {
  DoubleTopic getTopic();

  double get();

  double get(double defaultValue);

  @Override
  default double getAsDouble() {
    return get();
  }

  TimestampedDouble getAtomic();

  TimestampedDouble getAtomic(double defaultValue);

  @FunctionalInterface
  interface ValueListener {
    void accept(double value, long timestamp);
  }

  void setValueListener(ValueListener listener);
}
