/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <vector>

#include "frc/trajectory/Trajectory.h"
#include "frc/trajectory/TrajectoryGenerator.h"
#include "frc/trajectory/constraint/CentripetalAccelerationConstraint.h"
#include "frc/trajectory/constraint/TrajectoryConstraint.h"
#include "gtest/gtest.h"
#include "trajectory/TestTrajectory.h"

using namespace frc;

TEST(TrajectoryGenerationTest, ObeysConstraints) {
  TrajectoryConfig config{12_fps, 12_fps_sq};
  auto trajectory = TestTrajectory::GetTrajectory(config);

  units::second_t time = 0_s;
  units::second_t dt = 20_ms;
  units::second_t duration = trajectory.TotalTime();

  while (time < duration) {
    const Trajectory::State point = trajectory.Sample(time);
    time += dt;

    EXPECT_TRUE(units::math::abs(point.velocity) <= 12_fps + 0.01_fps);
    EXPECT_TRUE(units::math::abs(point.acceleration) <=
                12_fps_sq + 0.01_fps_sq);
  }
}
