package frc.sim.physics;

import java.util.Objects;

import edu.wpi.first.wpilibj.geometry.Rotation2d;

public class Force2d {
  public Vector2d vec;

  /**
   * Constructs a Force2d with X and Y components equal to zero.
   */
  public Force2d() {
    this(0.0, 0.0);
  }

  /**
   * Constructs a Force2d with the X and Y components equal to the
   * provided values.
   *
   * @param x The x component of the force.
   * @param y The y component of the force.
   */
  public Force2d( double x, double y) {
    vec = new Vector2d(x, y);
  }

  /**
   * Constructs a Force2d with the provided force magnitude and angle. This is
   * essentially converting from polar coordinates to Cartesian coordinates.
   *
   * @param mag The magnititude of the force 
   * @param angle    The angle from the x-axis to the force vector.
   */
  public Force2d(double mag, Rotation2d angle) {
    this(mag * angle.getCos(), mag * angle.getSin());
  }

  /**
   * Returns the X component of the force.
   *
   * @return The x component of the force.
   */

  public double getX() {
    return vec.x;
  }

  /**
   * Returns the Y component of the force.
   *
   * @return The y component of the force.
   */

  public double getY() {
    return vec.y;
  }

  /**
   * Returns the norm, or distance from the origin to the force.
   *
   * @return The norm of the force.
   */
  public double getNorm() {
    return Math.hypot(vec.x, vec.y);
  }

  /**
   * 
   * @return a unit vector in the directino this force points
   */
  public Vector2d getUnitVector() {
    return new Vector2d(this.getX()/this.getNorm(), this.getY()/this.getNorm());
  }

  /**
   * Applies a rotation to the force in 2d space.
   *
   * <p>This multiplies the force vector by a counterclockwise rotation
   * matrix of the given angle.
   * [x_new]   [other.cos, -other.sin][x]
   * [y_new] = [other.sin,  other.cos][y]
   *
   * <p>For example, rotating a Force2d of {2, 0} by 90 degrees will return a
   * Force2d of {0, 2}.
   *
   * @param other The rotation to rotate the force by.
   * @return The new rotated force.
   */
  public Force2d rotateBy(Rotation2d other) {
    return new Force2d(
            vec.x * other.getCos() - vec.y * other.getSin(),
            vec.x * other.getSin() + vec.y * other.getCos()
    );
  }

  /**
   * Adds two forces in 2d space and returns the sum. This is similar to
   * vector addition.
   *
   * <p>For example, Force2d{1.0, 2.5} + Force2d{2.0, 5.5} =
   * Force2d{3.0, 8.0}
   *
   * @param other The force to add.
   * @return The sum of the forces.
   */
  public Force2d plus(Force2d other) {
    return new Force2d(vec.x + other.vec.x, vec.y + other.vec.y);
  }

  /**
   * Accumulates another force into this force
   *
   *
   * @param other The force to add.
   * @return nothing (acts on this force in-place)
   */
  public void accum(Force2d other) {
    vec.x += other.vec.x;
    vec.y += other.vec.y;
  }

  /**
   * Subtracts the other force from the other force and returns the
   * difference.
   *
   * <p>For example, Force2d{5.0, 4.0} - Force2d{1.0, 2.0} =
   * Force2d{4.0, 2.0}
   *
   * @param other The force to subtract.
   * @return The difference between the two forces.
   */
  public Force2d minus(Force2d other) {
    return new Force2d(vec.x - other.vec.x, vec.y - other.vec.y);
  }

  /**
   * Returns the inverse of the current force. This is equivalent to
   * rotating by 180 degrees, flipping the point over both axes, or simply
   * negating both components of the force.
   *
   * @return The inverse of the current force.
   */
  public Force2d unaryMinus() {
    return new Force2d(-vec.x, -vec.y);
  }

  /**
   * Multiplies the force by a scalar and returns the new force.
   *
   * <p>For example, Force2d{2.0, 2.5} * 2 = Force2d{4.0, 5.0}
   *
   * @param scalar The scalar to multiply by.
   * @return The scaled force.
   */
  public Force2d times(double scalar) {
    return new Force2d(vec.x * scalar, vec.y * scalar);
  }

  /**
   * Divides the force by a scalar and returns the new force.
   *
   * <p>For example, Force2d{2.0, 2.5} / 2 = Force2d{1.0, 1.25}
   *
   * @param scalar The scalar to multiply by.
   * @return The reference to the new mutated object.
   */
  public Force2d div(double scalar) {
    return new Force2d(vec.x / scalar, vec.y / scalar);
  }

  @Override
  public String toString() {
    return String.format("Force2d(X: %.2f, Y: %.2f)", vec.x, vec.y);
  }

  /**
   * Checks equality between this Force2d and another object.
   *
   * @param obj The other object.
   * @return Whether the two objects are equal or not.
   */
  @Override
  public boolean equals(Object obj) {
    if (obj instanceof Force2d) {
      return Math.abs(((Force2d) obj).vec.x - vec.x) < 1E-9
          && Math.abs(((Force2d) obj).vec.y - vec.y) < 1E-9;
    }
    return false;
  }

  @Override
  public int hashCode() {
    return Objects.hash(vec.x, vec.y);
  }
}
