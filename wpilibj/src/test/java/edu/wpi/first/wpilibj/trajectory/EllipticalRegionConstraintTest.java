/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.trajectory;

import java.util.List;

import org.junit.jupiter.api.Test;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.geometry.Translation2d;
import edu.wpi.first.wpilibj.trajectory.constraint.EllipticalRegionConstraint;
import edu.wpi.first.wpilibj.trajectory.constraint.MaxVelocityConstraint;
import edu.wpi.first.wpilibj.util.Units;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class EllipticalRegionConstraintTest {
  @Test
  void testConstraint() {
    // Create constraints
    double maxVelocity = Units.feetToMeters(3.0);
    var maxVelocityConstraint = new MaxVelocityConstraint(maxVelocity);
    var regionConstraint = new EllipticalRegionConstraint(
        new Translation2d(Units.feetToMeters(5.0), Units.feetToMeters(5.0)),
        Units.feetToMeters(10.0), Units.feetToMeters(5.0), Rotation2d.fromDegrees(180.0),
        maxVelocityConstraint
    );

    // Get trajectory
    var trajectory = TrajectoryGeneratorTest.getTrajectory(List.of(regionConstraint));

    // Iterate through trajectory and check constraints
    boolean exceededConstraintOutsideRegion = false;
    for (double t = 0.0; t < trajectory.getTotalTimeSeconds(); t += 0.02) {
      var point = trajectory.sample(t);
      var translation = point.poseMeters.getTranslation();

      if (translation.getX() < Units.feetToMeters(10)
          && translation.getY() < Units.feetToMeters(5)) {
        assertTrue(Math.abs(point.velocityMetersPerSecond) < maxVelocity + 0.05);
      } else if (Math.abs(point.velocityMetersPerSecond) >= maxVelocity + 0.05) {
        exceededConstraintOutsideRegion = true;
      }
    }
    assertTrue(exceededConstraintOutsideRegion);
  }

  @Test
  void testIsPoseWithinRegion() {
    double maxVelocity = Units.feetToMeters(3.0);
    var maxVelocityConstraint = new MaxVelocityConstraint(maxVelocity);

    var regionConstraintNoRotation = new EllipticalRegionConstraint(
        new Translation2d(Units.feetToMeters(1.0), Units.feetToMeters(1.0)),
        Units.feetToMeters(2.0), Units.feetToMeters(4.0), new Rotation2d(),
        maxVelocityConstraint);

    assertFalse(regionConstraintNoRotation.isPoseWithinRegion(new Pose2d(
        Units.feetToMeters(2.1), Units.feetToMeters(1.0), new Rotation2d()
    )));

    var regionConstraintWithRotation = new EllipticalRegionConstraint(
        new Translation2d(Units.feetToMeters(1.0), Units.feetToMeters(1.0)),
        Units.feetToMeters(2.0), Units.feetToMeters(4.0), Rotation2d.fromDegrees(90.0),
        maxVelocityConstraint);

    assertTrue(regionConstraintWithRotation.isPoseWithinRegion(new Pose2d(
        Units.feetToMeters(2.1), Units.feetToMeters(1.0), new Rotation2d()
    )));
  }
}
