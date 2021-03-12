// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

namespace frc {

/**
 * Interface for potentiometers.
 */
class Potentiometer {
 public:
  Potentiometer() = default;
  virtual ~Potentiometer() = default;

  Potentiometer(Potentiometer&&) = default;
  Potentiometer& operator=(Potentiometer&&) = default;

  /**
   * Common interface for getting the current value of a potentiometer.
   *
   * @return The current set speed. Value is between -1.0 and 1.0.
   */
  virtual double Get() const = 0;
};

}  // namespace frc
