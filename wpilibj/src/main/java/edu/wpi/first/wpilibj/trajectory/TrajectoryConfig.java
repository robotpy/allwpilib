/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.trajectory;

import java.util.ArrayList;
import java.util.List;

import edu.wpi.first.wpilibj.kinematics.DifferentialDriveKinematics;
import edu.wpi.first.wpilibj.trajectory.constraint.DifferentialDriveKinematicsConstraint;
import edu.wpi.first.wpilibj.trajectory.constraint.TrajectoryConstraint;

/**
 * Represents the configuration for generating a trajectory. This class stores the start velocity,
 * end velocity, max velocity, max acceleration, custom constraints, and the reversed flag.
 *
 * <p>The class must be constructed with a max velocity and max acceleration. The other parameters
 * (start velocity, end velocity, constraints, reversed) have been defaulted to reasonable
 * values (0, 0, {}, false). These values can be changed via the setXXX methods.
 */
public class TrajectoryConfig {
  private final double m_maxVelocity;
  private final double m_maxAcceleration;
  private final List<TrajectoryConstraint> m_constraints;
  private double m_startVelocity;
  private double m_endVelocity;
  private boolean m_reversed;

  /**
   * Constructs the trajectory configuration class.
   *
   * @param maxVelocityMetersPerSecond       The max velocity for the trajectory.
   * @param maxAccelerationMetersPerSecondSq The max acceleration for the trajectory.
   */
  public TrajectoryConfig(double maxVelocityMetersPerSecond,
                          double maxAccelerationMetersPerSecondSq) {
    m_maxVelocity = maxVelocityMetersPerSecond;
    m_maxAcceleration = maxAccelerationMetersPerSecondSq;
    m_constraints = new ArrayList<>();
  }

  /**
   * Adds a user-defined constraint to the trajectory.
   *
   * @param constraint The user-defined constraint.
   * @return Instance of the current config object.
   */
  public TrajectoryConfig addConstraint(TrajectoryConstraint constraint) {
    m_constraints.add(constraint);
    return this;
  }

  /**
   * Adds all user-defined constraints from a list to the trajectory.
   * @param constraints List of user-defined constraints.
   * @return Instance of the current config object.
   */
  public TrajectoryConfig addConstraints(List<TrajectoryConstraint> constraints) {
    m_constraints.addAll(constraints);
    return this;
  }

  /**
   * Adds a differential drive kinematics constraint to ensure that
   * no wheel velocity of a differential drive goes above the max velocity.
   *
   * @param kinematics The differential drive kinematics.
   * @return Instance of the current config object.
   */
  public TrajectoryConfig setKinematics(DifferentialDriveKinematics kinematics) {
    addConstraint(new DifferentialDriveKinematicsConstraint(kinematics, m_maxVelocity));
    return this;
  }

  /**
   * Returns the starting velocity of the trajectory.
   *
   * @return The starting velocity of the trajectory in meters per second.
   */
  public double getStartVelocityMetersPerSecond() {
    return m_startVelocity;
  }

  /**
   * Sets the start velocity of the trajectory.
   *
   * @param startVelocityMetersPerSecond The start velocity of the trajectory.
   * @return Instance of the current config object.
   */
  public TrajectoryConfig setStartVelocity(double startVelocityMetersPerSecond) {
    m_startVelocity = startVelocityMetersPerSecond;
    return this;
  }

  /**
   * Returns the starting velocity of the trajectory.
   *
   * @return The starting velocity of the trajectory in meters per second.
   */
  public double getEndVelocityMetersPerSecond() {
    return m_endVelocity;
  }

  /**
   * Sets the end velocity of the trajectory.
   *
   * @param endVelocityMetersPerSecond The end velocity of the trajectory.
   * @return Instance of the current config object.
   */
  public TrajectoryConfig setEndVelocity(double endVelocityMetersPerSecond) {
    m_endVelocity = endVelocityMetersPerSecond;
    return this;
  }

  /**
   * Returns the maximum velocity of the trajectory.
   *
   * @return The maximum velocity of the trajectory in meters per second.
   */
  public double getMaxVelocityMetersPerSecond() {
    return m_maxVelocity;
  }

  /**
   * Returns the maximum acceleration of the trajectory.
   *
   * @return The maximum acceleration of the trajectory in meters per second squared.
   */
  public double getMaxAccelerationMetersPerSecondSq() {
    return m_maxAcceleration;
  }

  /**
   * Returns the user-defined constraints of the trajectory.
   *
   * @return The user-defined constraints of the trajectory.
   */
  public List<TrajectoryConstraint> getConstraints() {
    return m_constraints;
  }

  /**
   * Returns whether the trajectory is reversed or not.
   *
   * @return whether the trajectory is reversed or not.
   */
  public boolean isReversed() {
    return m_reversed;
  }

  /**
   * Sets the reversed flag of the trajectory.
   *
   * @param reversed Whether the trajectory should be reversed or not.
   * @return Instance of the current config object.
   */
  public TrajectoryConfig setReversed(boolean reversed) {
    m_reversed = reversed;
    return this;
  }
}
