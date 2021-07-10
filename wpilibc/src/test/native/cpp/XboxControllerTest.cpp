// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/XboxController.h"  // NOLINT(build/include_order)

#include "frc/simulation/XboxControllerSim.h"
#include "gtest/gtest.h"

using namespace frc;

TEST(XboxControllerTests, GetX) {
  XboxController joy{2};
  sim::XboxControllerSim joysim{joy};

  joysim.SetLeftX(0.35);
  joysim.SetRightX(0.45);
  joysim.NotifyNewData();
  ASSERT_NEAR(joy.GetLeftX(), 0.35, 0.001);
  ASSERT_NEAR(joy.GetRightX(), 0.45, 0.001);
}

TEST(XboxControllerTests, GetBumper) {
  XboxController joy{1};
  sim::XboxControllerSim joysim{joy};

  joysim.SetLeftBumper(false);
  joysim.SetRightBumper(true);
  joysim.NotifyNewData();
  ASSERT_FALSE(joy.GetLeftBumper());
  ASSERT_TRUE(joy.GetRightBumper());
  // need to call pressed and released to clear flags
  joy.GetLeftBumperPressed();
  joy.GetLeftBumperReleased();
  joy.GetRightBumperPressed();
  joy.GetRightBumperReleased();

  joysim.SetLeftBumper(true);
  joysim.SetRightBumper(false);
  joysim.NotifyNewData();
  ASSERT_TRUE(joy.GetLeftBumper());
  ASSERT_TRUE(joy.GetLeftBumperPressed());
  ASSERT_FALSE(joy.GetLeftBumperReleased());
  ASSERT_FALSE(joy.GetRightBumper());
  ASSERT_FALSE(joy.GetRightBumperPressed());
  ASSERT_TRUE(joy.GetRightBumperReleased());
}

TEST(XboxControllerTests, GetAButton) {
  XboxController joy{1};
  sim::XboxControllerSim joysim{joy};

  joysim.SetAButton(false);
  joysim.NotifyNewData();
  ASSERT_FALSE(joy.GetAButton());
  // need to call pressed and released to clear flags
  joy.GetAButtonPressed();
  joy.GetAButtonReleased();

  joysim.SetAButton(true);
  joysim.NotifyNewData();
  ASSERT_TRUE(joy.GetAButton());
  ASSERT_TRUE(joy.GetAButtonPressed());
  ASSERT_FALSE(joy.GetAButtonReleased());

  joysim.SetAButton(false);
  joysim.NotifyNewData();
  ASSERT_FALSE(joy.GetAButton());
  ASSERT_FALSE(joy.GetAButtonPressed());
  ASSERT_TRUE(joy.GetAButtonReleased());
}
