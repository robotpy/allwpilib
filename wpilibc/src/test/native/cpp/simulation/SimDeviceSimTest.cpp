// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <hal/SimDevice.h>
#include <wpi/StringRef.h>

#include "frc/simulation/SimDeviceSim.h"
#include "gtest/gtest.h"

using namespace frc::sim;

TEST(SimDeviceSimTests, TestBasic) {
  hal::SimDevice dev{"test"};
  hal::SimBoolean devBool = dev.CreateBoolean("bool", false, false);

  SimDeviceSim sim{"test"};
  hal::SimBoolean simBool = sim.GetBoolean("bool");
  EXPECT_FALSE(simBool.Get());
  simBool.Set(true);
  EXPECT_TRUE(devBool.Get());
}

TEST(SimDeviceSimTests, TestEnumerateDevices) {
  hal::SimDevice dev{"test"};

  bool foundit = false;
  SimDeviceSim::EnumerateDevices(
      "te", [&](const char* name, HAL_SimDeviceHandle handle) {
        if (wpi::StringRef(name) == "test")
          foundit = true;
      });
  EXPECT_TRUE(foundit);
}
