/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/geometry/Rotation2d.h"
#include "frc/interpolatable/TimeInterpolatableBuffer.h"
#include "gtest/gtest.h"
#include "units/time.h"

TEST(TimeInterpolatableBufferTest, TestInterpolation) {
  frc::TimeInterpolatableBuffer<frc::Rotation2d> buffer{};

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
