/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/trajectory/TrapezoidProfile.h"  // NOLINT(build/include_order)

#include <chrono>
#include <cmath>

#include "gtest/gtest.h"

static constexpr auto kDt = 10_ms;

#define EXPECT_NEAR_UNITS(val1, val2, eps) \
  EXPECT_LE(units::math::abs(val1 - val2), eps)

#define EXPECT_LT_OR_NEAR_UNITS(val1, val2, eps) \
  if (val1 <= val2) {                            \
    EXPECT_LE(val1, val2);                       \
  } else {                                       \
    EXPECT_NEAR_UNITS(val1, val2, eps);          \
  }

TEST(TrapezoidProfileTest, ReachesGoal) {
  frc::TrapezoidProfile::Constraints constraints{1.75_mps, 0.75_mps_sq};
  frc::TrapezoidProfile::State goal{3_m, 0_mps};
  frc::TrapezoidProfile::State state;

  for (int i = 0; i < 450; ++i) {
    frc::TrapezoidProfile profile{constraints, goal, state};
    state = profile.Calculate(kDt);
  }
  EXPECT_EQ(state, goal);
}

// Tests that decreasing the maximum velocity in the middle when it is already
// moving faster than the new max is handled correctly
TEST(TrapezoidProfileTest, PosContinousUnderVelChange) {
  frc::TrapezoidProfile::Constraints constraints{1.75_mps, 0.75_mps_sq};
  frc::TrapezoidProfile::State goal{12_m, 0_mps};

  frc::TrapezoidProfile profile{constraints, goal};
  auto state = profile.Calculate(kDt);

  auto lastPos = state.position;
  for (int i = 0; i < 1600; ++i) {
    if (i == 400) {
      constraints.maxVelocity = 0.75_mps;
    }

    profile = frc::TrapezoidProfile{constraints, goal, state};
    state = profile.Calculate(kDt);
    auto estimatedVel = (state.position - lastPos) / kDt;

    if (i >= 400) {
      // Since estimatedVel can have floating point rounding errors, we check
      // whether value is less than or within an error delta of the new
      // constraint.
      EXPECT_LT_OR_NEAR_UNITS(estimatedVel, constraints.maxVelocity, 1e-4_mps);

      EXPECT_LE(state.velocity, constraints.maxVelocity);
    }

    lastPos = state.position;
  }
  EXPECT_EQ(state, goal);
}

// There is some somewhat tricky code for dealing with going backwards
TEST(TrapezoidProfileTest, Backwards) {
  frc::TrapezoidProfile::Constraints constraints{0.75_mps, 0.75_mps_sq};
  frc::TrapezoidProfile::State goal{-2_m, 0_mps};
  frc::TrapezoidProfile::State state;

  for (int i = 0; i < 400; ++i) {
    frc::TrapezoidProfile profile{constraints, goal, state};
    state = profile.Calculate(kDt);
  }
  EXPECT_EQ(state, goal);
}

TEST(TrapezoidProfileTest, SwitchGoalInMiddle) {
  frc::TrapezoidProfile::Constraints constraints{0.75_mps, 0.75_mps_sq};
  frc::TrapezoidProfile::State goal{-2_m, 0_mps};
  frc::TrapezoidProfile::State state;

  for (int i = 0; i < 200; ++i) {
    frc::TrapezoidProfile profile{constraints, goal, state};
    state = profile.Calculate(kDt);
  }
  EXPECT_NE(state, goal);

  goal = {0.0_m, 0.0_mps};
  for (int i = 0; i < 550; ++i) {
    frc::TrapezoidProfile profile{constraints, goal, state};
    state = profile.Calculate(kDt);
  }
  EXPECT_EQ(state, goal);
}

// Checks to make sure that it hits top speed
TEST(TrapezoidProfileTest, TopSpeed) {
  frc::TrapezoidProfile::Constraints constraints{0.75_mps, 0.75_mps_sq};
  frc::TrapezoidProfile::State goal{4_m, 0_mps};
  frc::TrapezoidProfile::State state;

  for (int i = 0; i < 200; ++i) {
    frc::TrapezoidProfile profile{constraints, goal, state};
    state = profile.Calculate(kDt);
  }
  EXPECT_NEAR_UNITS(constraints.maxVelocity, state.velocity, 10e-5_mps);

  for (int i = 0; i < 2000; ++i) {
    frc::TrapezoidProfile profile{constraints, goal, state};
    state = profile.Calculate(kDt);
  }
  EXPECT_EQ(state, goal);
}

TEST(TrapezoidProfileTest, TimingToCurrent) {
  frc::TrapezoidProfile::Constraints constraints{0.75_mps, 0.75_mps_sq};
  frc::TrapezoidProfile::State goal{2_m, 0_mps};
  frc::TrapezoidProfile::State state;

  for (int i = 0; i < 400; i++) {
    frc::TrapezoidProfile profile{constraints, goal, state};
    state = profile.Calculate(kDt);
    EXPECT_NEAR_UNITS(profile.TimeLeftUntil(state.position), 0_s, 2e-2_s);
  }
}

TEST(TrapezoidProfileTest, TimingToGoal) {
  using units::unit_cast;

  frc::TrapezoidProfile::Constraints constraints{0.75_mps, 0.75_mps_sq};
  frc::TrapezoidProfile::State goal{2_m, 0_mps};

  frc::TrapezoidProfile profile{constraints, goal};
  auto state = profile.Calculate(kDt);

  auto predictedTimeLeft = profile.TimeLeftUntil(goal.position);
  bool reachedGoal = false;
  for (int i = 0; i < 400; i++) {
    profile = frc::TrapezoidProfile(constraints, goal, state);
    state = profile.Calculate(kDt);
    if (!reachedGoal && state == goal) {
      // Expected value using for loop index is just an approximation since the
      // time left in the profile doesn't increase linearly at the endpoints
      EXPECT_NEAR(unit_cast<double>(predictedTimeLeft), i / 100.0, 0.25);
      reachedGoal = true;
    }
  }
}

TEST(TrapezoidProfileTest, TimingBeforeGoal) {
  using units::unit_cast;

  frc::TrapezoidProfile::Constraints constraints{0.75_mps, 0.75_mps_sq};
  frc::TrapezoidProfile::State goal{2_m, 0_mps};

  frc::TrapezoidProfile profile{constraints, goal};
  auto state = profile.Calculate(kDt);

  auto predictedTimeLeft = profile.TimeLeftUntil(1_m);
  bool reachedGoal = false;
  for (int i = 0; i < 400; i++) {
    profile = frc::TrapezoidProfile(constraints, goal, state);
    state = profile.Calculate(kDt);
    if (!reachedGoal &&
        (units::math::abs(state.velocity - 1_mps) < 10e-5_mps)) {
      EXPECT_NEAR(unit_cast<double>(predictedTimeLeft), i / 100.0, 2e-2);
      reachedGoal = true;
    }
  }
}

TEST(TrapezoidProfileTest, TimingToNegativeGoal) {
  using units::unit_cast;

  frc::TrapezoidProfile::Constraints constraints{0.75_mps, 0.75_mps_sq};
  frc::TrapezoidProfile::State goal{-2_m, 0_mps};

  frc::TrapezoidProfile profile{constraints, goal};
  auto state = profile.Calculate(kDt);

  auto predictedTimeLeft = profile.TimeLeftUntil(goal.position);
  bool reachedGoal = false;
  for (int i = 0; i < 400; i++) {
    profile = frc::TrapezoidProfile(constraints, goal, state);
    state = profile.Calculate(kDt);
    if (!reachedGoal && state == goal) {
      // Expected value using for loop index is just an approximation since the
      // time left in the profile doesn't increase linearly at the endpoints
      EXPECT_NEAR(unit_cast<double>(predictedTimeLeft), i / 100.0, 0.25);
      reachedGoal = true;
    }
  }
}

TEST(TrapezoidProfileTest, TimingBeforeNegativeGoal) {
  using units::unit_cast;

  frc::TrapezoidProfile::Constraints constraints{0.75_mps, 0.75_mps_sq};
  frc::TrapezoidProfile::State goal{-2_m, 0_mps};

  frc::TrapezoidProfile profile{constraints, goal};
  auto state = profile.Calculate(kDt);

  auto predictedTimeLeft = profile.TimeLeftUntil(-1_m);
  bool reachedGoal = false;
  for (int i = 0; i < 400; i++) {
    profile = frc::TrapezoidProfile(constraints, goal, state);
    state = profile.Calculate(kDt);
    if (!reachedGoal &&
        (units::math::abs(state.velocity + 1_mps) < 10e-5_mps)) {
      EXPECT_NEAR(unit_cast<double>(predictedTimeLeft), i / 100.0, 2e-2);
      reachedGoal = true;
    }
  }
}
