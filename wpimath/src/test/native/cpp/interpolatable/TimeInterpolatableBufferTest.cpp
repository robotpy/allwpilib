/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <cmath>

#include "frc/geometry/Pose2d.h"
#include "frc/geometry/Rotation2d.h"
#include "frc/interpolatable/TimeInterpolatableBuffer.h"
#include "gtest/gtest.h"
#include "units/time.h"

TEST(TimeInterpolatableBufferTest, TestInterpolation) {
  frc::TimeInterpolatableBuffer<frc::Rotation2d> buffer{10_s};

  buffer.AddSample(0_s, frc::Rotation2d(0_rad));
  EXPECT_TRUE(buffer.Sample(0_s) == frc::Rotation2d(0_rad));
  buffer.AddSample(1_s, frc::Rotation2d(1_rad));
  EXPECT_TRUE(buffer.Sample(0.5_s) == frc::Rotation2d(0.5_rad));
  EXPECT_TRUE(buffer.Sample(1_s) == frc::Rotation2d(1_rad));
  buffer.AddSample(3_s, frc::Rotation2d(2_rad));
  EXPECT_TRUE(buffer.Sample(2_s) == frc::Rotation2d(1.5_rad));

  buffer.AddSample(10.5_s, frc::Rotation2d(2_rad));
  EXPECT_TRUE(buffer.Sample(0_s) == frc::Rotation2d(1_rad));
}

TEST(TimeInterpolatableBufferTest, TestPose2d) {
  frc::TimeInterpolatableBuffer<frc::Pose2d> buffer{10_s};
  // We expect to be at (1 - 1/std::sqrt(2), 1/std::sqrt(2), 45deg) at t=0.5
  buffer.AddSample(0_s, frc::Pose2d{0_m, 0_m, 90_deg});
  buffer.AddSample(1_s, frc::Pose2d{1_m, 1_m, 0_deg});
  frc::Pose2d sample = buffer.Sample(0.5_s);
  EXPECT_TRUE(std::abs(sample.X().to<double>() - (1 - 1 / std::sqrt(2))) <
              0.01);
  EXPECT_TRUE(std::abs(sample.Y().to<double>() - (1 / std::sqrt(2))) < 0.01);
  EXPECT_TRUE(std::abs(sample.Rotation().Degrees().to<double>() - 45) < 0.01);
}
