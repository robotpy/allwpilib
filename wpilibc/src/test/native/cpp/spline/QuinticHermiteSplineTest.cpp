/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <chrono>
#include <iostream>

#include <units/units.h>

#include "frc/geometry/Pose2d.h"
#include "frc/geometry/Rotation2d.h"
#include "frc/spline/QuinticHermiteSpline.h"
#include "frc/spline/SplineHelper.h"
#include "frc/spline/SplineParameterizer.h"
#include "gtest/gtest.h"

using namespace frc;

namespace frc {
class QuinticHermiteSplineTest : public ::testing::Test {
 protected:
  static void Run(const Pose2d& a, const Pose2d& b) {
    // Start the timer.
    const auto start = std::chrono::high_resolution_clock::now();

    // Generate and parameterize the spline.
    const auto spline = SplineHelper::QuinticSplinesFromControlVectors(
        SplineHelper::QuinticControlVectorsFromWaypoints({a, b}))[0];
    const auto poses = SplineParameterizer::Parameterize(spline);

    // End timer.
    const auto finish = std::chrono::high_resolution_clock::now();

    // Calculate the duration (used when benchmarking)
    const auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

    for (unsigned int i = 0; i < poses.size() - 1; i++) {
      auto& p0 = poses[i];
      auto& p1 = poses[i + 1];

      // Make sure the twist is under the tolerance defined by the Spline class.
      auto twist = p0.first.Log(p1.first);
      EXPECT_LT(std::abs(twist.dx.to<double>()),
                SplineParameterizer::kMaxDx.to<double>());
      EXPECT_LT(std::abs(twist.dy.to<double>()),
                SplineParameterizer::kMaxDy.to<double>());
      EXPECT_LT(std::abs(twist.dtheta.to<double>()),
                SplineParameterizer::kMaxDtheta.to<double>());
    }

    // Check first point.
    EXPECT_NEAR(poses.front().first.Translation().X().to<double>(),
                a.Translation().X().to<double>(), 1E-9);
    EXPECT_NEAR(poses.front().first.Translation().Y().to<double>(),
                a.Translation().Y().to<double>(), 1E-9);
    EXPECT_NEAR(poses.front().first.Rotation().Radians().to<double>(),
                a.Rotation().Radians().to<double>(), 1E-9);

    // Check last point.
    EXPECT_NEAR(poses.back().first.Translation().X().to<double>(),
                b.Translation().X().to<double>(), 1E-9);
    EXPECT_NEAR(poses.back().first.Translation().Y().to<double>(),
                b.Translation().Y().to<double>(), 1E-9);
    EXPECT_NEAR(poses.back().first.Rotation().Radians().to<double>(),
                b.Rotation().Radians().to<double>(), 1E-9);

    static_cast<void>(duration);
  }
};
}  // namespace frc

TEST_F(QuinticHermiteSplineTest, StraightLine) {
  Run(Pose2d(), Pose2d(3_m, 0_m, Rotation2d()));
}

TEST_F(QuinticHermiteSplineTest, SimpleSCurve) {
  Run(Pose2d(), Pose2d(1_m, 1_m, Rotation2d()));
}

TEST_F(QuinticHermiteSplineTest, SquigglyCurve) {
  Run(Pose2d(0_m, 0_m, Rotation2d(90_deg)),
      Pose2d(-1_m, 0_m, Rotation2d(90_deg)));
}
