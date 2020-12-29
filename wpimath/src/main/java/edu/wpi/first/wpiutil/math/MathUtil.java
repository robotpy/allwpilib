// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpiutil.math;

public final class MathUtil {
  private MathUtil() {
    throw new AssertionError("utility class");
  }

  /**
   * Returns value clamped between low and high boundaries.
   *
   * @param value Value to clamp.
   * @param low   The lower boundary to which to clamp value.
   * @param high  The higher boundary to which to clamp value.
   */
  public static int clamp(int value, int low, int high) {
    return Math.max(low, Math.min(value, high));
  }

  /**
   * Returns value clamped between low and high boundaries.
   *
   * @param value Value to clamp.
   * @param low   The lower boundary to which to clamp value.
   * @param high  The higher boundary to which to clamp value.
   */
  public static double clamp(double value, double low, double high) {
    return Math.max(low, Math.min(value, high));
  }

  /**
   * Constrains theta to within the range (-pi, pi].
   *
   * @param theta The angle to normalize.
   * @return The normalized angle.
   */
  @SuppressWarnings("LocalVariableName")
  public static double normalizeAngle(double theta) {
    // Constraint theta to within (-3pi, pi)
    int nPiPos = (int) ((theta + Math.PI) / 2.0 / Math.PI);
    theta -= nPiPos * 2.0 * Math.PI;

    // Cut off the bottom half of the above range to constrain within
    // (-pi, pi]
    int nPiNeg = (int) ((theta - Math.PI) / 2.0 / Math.PI);
    theta -= nPiNeg * 2.0 * Math.PI;

    return theta;
  }

  /**
   * Perform linear interpolation between two values.
   * @param startValue The value to start at.
   * @param endValue The value to end at.
   * @param t How far between the two values to interpolate. This is clamped to [0, 1].
   */
  @SuppressWarnings("ParameterName")
  public static double interpolate(double startValue, double endValue, double t) {
    return startValue + (endValue - startValue) * MathUtil.clamp(t, 0, 1);
  }
}
