/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.controller;

import java.util.ArrayList;

import org.junit.jupiter.api.Test;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.geometry.Twist2d;
import edu.wpi.first.wpilibj.trajectory.TrajectoryConfig;
import edu.wpi.first.wpilibj.trajectory.TrajectoryGenerator;

import static org.junit.jupiter.api.Assertions.assertAll;
import static org.junit.jupiter.api.Assertions.assertEquals;

class RamseteControllerTest {
  private static final double kTolerance = 1 / 12.0;
  private static final double kAngularTolerance = Math.toRadians(2);

  private static double boundRadians(double value) {
    while (value > Math.PI) {
      value -= Math.PI * 2;
    }
    while (value <= -Math.PI) {
      value += Math.PI * 2;
    }
    return value;
  }

  @Test
  @SuppressWarnings("PMD.AvoidInstantiatingObjectsInLoops")
  void testReachesReference() {
    final var controller = new RamseteController(2.0, 0.7);
    var robotPose = new Pose2d(2.7, 23.0, Rotation2d.fromDegrees(0.0));

    final var waypoints = new ArrayList<Pose2d>();
    waypoints.add(new Pose2d(2.75, 22.521, new Rotation2d(0)));
    waypoints.add(new Pose2d(24.73, 19.68, new Rotation2d(5.846)));
    var config = new TrajectoryConfig(8.8, 0.1);
    final var trajectory = TrajectoryGenerator.generateTrajectory(waypoints, config);

    final double kDt = 0.02;
    final var totalTime = trajectory.getTotalTimeSeconds();
    for (int i = 0; i < (totalTime / kDt); ++i) {
      var state = trajectory.sample(kDt * i);

      var output = controller.calculate(robotPose, state);
      robotPose = robotPose.exp(new Twist2d(output.vxMetersPerSecond * kDt, 0,
          output.omegaRadiansPerSecond * kDt));
    }

    final var states = trajectory.getStates();
    final var endPose = states.get(states.size() - 1).poseMeters;

    // Java lambdas require local variables referenced from a lambda expression
    // must be final or effectively final.
    final var finalRobotPose = robotPose;
    assertAll(
        () -> assertEquals(endPose.getTranslation().getX(), finalRobotPose.getTranslation().getX(),
            kTolerance),
        () -> assertEquals(endPose.getTranslation().getY(), finalRobotPose.getTranslation().getY(),
            kTolerance),
        () -> assertEquals(0.0,
            boundRadians(endPose.getRotation().getRadians()
                - finalRobotPose.getRotation().getRadians()),
            kAngularTolerance)
    );
  }
}
