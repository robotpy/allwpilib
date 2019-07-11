/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "frc/PWMSpeedController.h"

namespace frc {

/**
 * REV Robotics SparkMax Speed Controller.
 */
class SparkMax : public PWMSpeedController {
 public:
  /**
   * Constructor for a Spark.
   *
   * @param channel The PWM channel that the Spark is attached to. 0-9 are
   *                on-board, 10-19 are on the MXP port
   */
  explicit SparkMax(int channel);

  SparkMax(SparkMax&&) = default;
  SparkMax& operator=(SparkMax&&) = default;
};

}  // namespace frc
