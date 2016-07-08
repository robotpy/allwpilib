/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008-2016. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "SensorBase.h"

#include "HAL/HAL.h"
#include "WPIErrors.h"

const uint32_t SensorBase::kDigitalChannels;
const uint32_t SensorBase::kAnalogInputs;
const uint32_t SensorBase::kSolenoidChannels;
const uint32_t SensorBase::kSolenoidModules;
const uint32_t SensorBase::kPwmChannels;
const uint32_t SensorBase::kRelayChannels;
const uint32_t SensorBase::kPDPChannels;
const uint32_t SensorBase::kChassisSlots;

/**
 * Check that the solenoid module number is valid.
 *
 * @return Solenoid module is valid and present
 */
bool SensorBase::CheckSolenoidModule(uint8_t moduleNumber) {
  if (moduleNumber < 64) return true;
  return false;
}

/**
 * Check that the digital channel number is valid.
 *
 * Verify that the channel number is one of the legal channel numbers. Channel
 * numbers are 1-based.
 *
 * @return Digital channel is valid
 */
bool SensorBase::CheckDigitalChannel(uint32_t channel) {
  if (channel < kDigitalChannels) return true;
  return false;
}

/**
 * Check that the relay channel number is valid.
 *
 * Verify that the channel number is one of the legal channel numbers. Channel
 * numbers are 0-based.
 *
 * @return Relay channel is valid
 */
bool SensorBase::CheckRelayChannel(uint32_t channel) {
  return checkRelayChannel((uint8_t)channel);
}

/**
 * Check that the digital channel number is valid.
 *
 * Verify that the channel number is one of the legal channel numbers. Channel
 * numbers are 1-based.
 *
 * @return PWM channel is valid
 */
bool SensorBase::CheckPWMChannel(uint32_t channel) {
  if (channel < kPwmChannels) return true;
  return false;
}

/**
 * Check that the analog input number is value.
 *
 * Verify that the analog input number is one of the legal channel numbers.
 * Channel numbers are 0-based.
 *
 * @return Analog channel is valid
 */
bool SensorBase::CheckAnalogInput(uint32_t channel) {
  if (channel < kAnalogInputs) return true;
  return false;
}

/**
 * Check that the analog output number is valid.
 *
 * Verify that the analog output number is one of the legal channel numbers.
 * Channel numbers are 0-based.
 *
 * @return Analog channel is valid
 */
bool SensorBase::CheckAnalogOutput(uint32_t channel) {
  if (channel < kAnalogOutputs) return true;
  return false;
}

/**
 * Verify that the solenoid channel number is within limits.
 *
 * @return Solenoid channel is valid
 */
bool SensorBase::CheckSolenoidChannel(uint32_t channel) {
  if (channel < kSolenoidChannels) return true;
  return false;
}

/**
 * Verify that the power distribution channel number is within limits.
 *
 * @return PDP channel is valid
 */
bool SensorBase::CheckPDPChannel(uint32_t channel) {
  if (channel < kPDPChannels) return true;
  return false;
}
