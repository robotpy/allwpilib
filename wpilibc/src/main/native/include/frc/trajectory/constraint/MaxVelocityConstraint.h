/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <units/units.h>

#include "frc/trajectory/constraint/TrajectoryConstraint.h"

namespace frc {
/**
 * Represents a constraint that enforces a max velocity. This can be coupled
 * with the EllipticalRegionConstraint or RectangularRegionConstraint to enforce
 * a max velocity within a region.
 */
class MaxVelocityConstraint : public TrajectoryConstraint {
 public:
  /**
   * Constructs a new MaxVelocityConstraint.
   *
   * @param maxVelocity The max velocity.
   */
  explicit MaxVelocityConstraint(units::meters_per_second_t maxVelocity)
      : m_maxVelocity(units::math::abs(maxVelocity)) {}

  units::meters_per_second_t MaxVelocity(
      const Pose2d& pose, units::curvature_t curvature,
      units::meters_per_second_t velocity) override {
    return m_maxVelocity;
  }

  MinMax MinMaxAcceleration(const Pose2d& pose, units::curvature_t curvature,
                            units::meters_per_second_t speed) override {
    return {};
  }

 private:
  units::meters_per_second_t m_maxVelocity;
};
}  // namespace frc
