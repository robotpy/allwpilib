// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "gtest/gtest.h"
#include "hal/HAL.h"
#include "hal/Solenoid.h"
#include "hal/handles/HandlesInternal.h"
#include "hal/simulation/CTREPCMData.h"

namespace hal {

std::string gTestSolenoidCallbackName;
HAL_Value gTestSolenoidCallbackValue;

void TestSolenoidInitializationCallback(const char* name, void* param,
                                        const struct HAL_Value* value) {
  gTestSolenoidCallbackName = name;
  gTestSolenoidCallbackValue = *value;
}

TEST(SolenoidSimTests, TestSolenoidInitialization) {
  const int MODULE_TO_TEST = 2;
  const int CHANNEL_TO_TEST = 3;

  int callbackParam = 0;
  int callbackId = HALSIM_RegisterCTREPCMSolenoidInitializedCallback(
      MODULE_TO_TEST, CHANNEL_TO_TEST, &TestSolenoidInitializationCallback,
      &callbackParam, false);
  ASSERT_TRUE(0 != callbackId);

  int32_t status;
  HAL_PortHandle portHandle;
  HAL_DigitalHandle solenoidHandle;

  // Use out of range index
  status = 0;
  portHandle = 8000;
  gTestSolenoidCallbackName = "Unset";
  solenoidHandle = HAL_InitializeSolenoidPort(portHandle, &status);
  EXPECT_EQ(HAL_kInvalidHandle, solenoidHandle);
  EXPECT_EQ(HAL_HANDLE_ERROR, status);
  EXPECT_STREQ("Unset", gTestSolenoidCallbackName.c_str());

  // Successful setup
  status = 0;
  portHandle = HAL_GetPortWithModule(MODULE_TO_TEST, CHANNEL_TO_TEST);
  gTestSolenoidCallbackName = "Unset";
  solenoidHandle = HAL_InitializeSolenoidPort(portHandle, &status);
  EXPECT_TRUE(HAL_kInvalidHandle != solenoidHandle);
  EXPECT_EQ(0, status);
  EXPECT_STREQ("SolenoidInitialized", gTestSolenoidCallbackName.c_str());

  // Double initialize... should fail
  status = 0;
  portHandle = HAL_GetPortWithModule(MODULE_TO_TEST, CHANNEL_TO_TEST);
  gTestSolenoidCallbackName = "Unset";
  solenoidHandle = HAL_InitializeSolenoidPort(portHandle, &status);
  EXPECT_EQ(HAL_kInvalidHandle, solenoidHandle);
  EXPECT_EQ(NO_AVAILABLE_RESOURCES, status);
  EXPECT_STREQ("Unset", gTestSolenoidCallbackName.c_str());

  // Reset, should allow you to re-register
  hal::HandleBase::ResetGlobalHandles();
  HALSIM_ResetCTREPCMData(MODULE_TO_TEST);
  callbackId = HALSIM_RegisterCTREPCMSolenoidInitializedCallback(
      MODULE_TO_TEST, CHANNEL_TO_TEST, &TestSolenoidInitializationCallback,
      &callbackParam, false);
  ASSERT_TRUE(0 != callbackId);

  status = 0;
  portHandle = HAL_GetPortWithModule(MODULE_TO_TEST, CHANNEL_TO_TEST);
  gTestSolenoidCallbackName = "Unset";
  solenoidHandle = HAL_InitializeSolenoidPort(portHandle, &status);
  EXPECT_TRUE(HAL_kInvalidHandle != solenoidHandle);
  EXPECT_EQ(0, status);
  EXPECT_STREQ("SolenoidInitialized", gTestSolenoidCallbackName.c_str());
}

}  // namespace hal
