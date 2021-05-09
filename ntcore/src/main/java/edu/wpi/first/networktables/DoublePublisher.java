// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.networktables;

import java.util.function.DoubleConsumer;

/** NetworkTables double subscriber. */
public interface DoublePublisher extends Publisher, DoubleConsumer {
  DoubleTopic getTopic();

  default void set(double value) {
    set(value, 0);
  }

  void set(double value, long time);

  default void setDefault(double value) {
    setDefault(value, 0);
  }

  void setDefault(double value, long time);

  @Override
  default void accept(double value) {
    set(value);
  }
}
