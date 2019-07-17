/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "Translation2d.h"
#include "Transform2d.h"
#include "Twist2d.h"

namespace frc {

/**
 * Represents a 2d pose containing translational and rotational elements.
 */
class Pose2d {
 public:
  /**
   * Constructs a pose at the origin facing toward the positive X axis.
   * (Translation2d{0, 0} and Rotation{0})
   */
  Pose2d() = default;

  /**
   * Constructs a pose with the specified translation and rotation.
   */
  Pose2d(Translation2d translation, Rotation2d rotation);

  /**
   * Convenience constructors that takes in x and y values directly instead of
   * having to construct a Translation2d.
   */
  Pose2d(double x, double y, Rotation2d rotation);

  /**
   * Transforms the pose by the given transformation and returns the new
   * transformed pose.
   *
   * [x_new] += [cos, -sin, 0][transform.x]
   * [y_new] += [sin,  cos, 0][transform.y]
   * [t_new] += [0,    0,   1][transform.t]
   */
  Pose2d operator+(const Transform2d& other) const;

  /**
   * Transforms the current pose by the transformation.
   *
   * This is similar to the + operator, except that it mutates the current
   * object.
   */
  Pose2d& operator+=(const Transform2d& other);

  /**
   * Returns the underlying translation.
   */
  const Translation2d& Translation() const { return m_translation; }

  /**
   * Returns the underlying rotation.
   */
  const Rotation2d& Rotation() const { return m_rotation; }

  /**
   * Transforms the pose by the given transformation and returns the new pose.
   * See + operator for the matrix multiplication performed.
   */
  Pose2d TransformBy(const Transform2d& other) const;

  /**
   * Returns the other pose relative to the current pose.
   *
   * This function can often be used for trajectory tracking or pose
   * stabilization algorithms to get the error between the reference and the
   * current pose.
   */
  Pose2d RelativeTo(const Pose2d& other) const;

  /**
   * Obtain a new Pose2d from a (constant curvature) velocity.
   *
   * See <https://file.tavsys.net/control/state-space-guide.pdf> section on
   * nonlinear pose estimation for derivation.
   */
  Pose2d Exp(const Twist2d& twist) const;

 private:
  Translation2d m_translation;
  Rotation2d m_rotation;
};
}  // namespace frc
