// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/XboxController.h"

#include <hal/FRCUsageReporting.h>

using namespace frc;

XboxController::XboxController(int port) : GenericHID(port) {
  HAL_Report(HALUsageReporting::kResourceType_XboxController, port + 1);
}

double XboxController::GetLeftX() const {
  return GetRawAxis(static_cast<int>(Axis::kLeftX));
}

double XboxController::GetRightX() const {
  return GetRawAxis(static_cast<int>(Axis::kRightX));
}

double XboxController::GetLeftY() const {
  return GetRawAxis(static_cast<int>(Axis::kLeftY));
}

double XboxController::GetRightY() const {
  return GetRawAxis(static_cast<int>(Axis::kRightY));
}

double XboxController::GetLeftTriggerAxis() const {
  return GetRawAxis(static_cast<int>(Axis::kLeftTrigger));
}

double XboxController::GetRightTriggerAxis() const {
  return GetRawAxis(static_cast<int>(Axis::kRightTrigger));
}

bool XboxController::GetLeftBumper() const {
  return GetRawButton(static_cast<int>(Button::kBumperLeft));
}

bool XboxController::GetRightBumper() const {
  return GetRawButton(static_cast<int>(Button::kBumperRight));
}

bool XboxController::GetLeftBumperPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kBumperLeft));
}

bool XboxController::GetRightBumperPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kBumperRight));
}

bool XboxController::GetLeftBumperReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kBumperLeft));
}

bool XboxController::GetRightBumperReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kBumperRight));
}

bool XboxController::GetLeftStickButton() const {
  return GetRawButton(static_cast<int>(Button::kStickLeft));
}

bool XboxController::GetRightStickButton() const {
  return GetRawButton(static_cast<int>(Button::kStickRight));
}

bool XboxController::GetLeftStickButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kStickLeft));
}

bool XboxController::GetRightStickButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kStickRight));
}

bool XboxController::GetLeftStickButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kStickLeft));
}

bool XboxController::GetRightStickButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kStickRight));
}

bool XboxController::GetAButton() const {
  return GetRawButton(static_cast<int>(Button::kA));
}

bool XboxController::GetAButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kA));
}

bool XboxController::GetAButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kA));
}

bool XboxController::GetBButton() const {
  return GetRawButton(static_cast<int>(Button::kB));
}

bool XboxController::GetBButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kB));
}

bool XboxController::GetBButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kB));
}

bool XboxController::GetXButton() const {
  return GetRawButton(static_cast<int>(Button::kX));
}

bool XboxController::GetXButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kX));
}

bool XboxController::GetXButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kX));
}

bool XboxController::GetYButton() const {
  return GetRawButton(static_cast<int>(Button::kY));
}

bool XboxController::GetYButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kY));
}

bool XboxController::GetYButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kY));
}

bool XboxController::GetBackButton() const {
  return GetRawButton(static_cast<int>(Button::kBack));
}

bool XboxController::GetBackButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kBack));
}

bool XboxController::GetBackButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kBack));
}

bool XboxController::GetStartButton() const {
  return GetRawButton(static_cast<int>(Button::kStart));
}

bool XboxController::GetStartButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kStart));
}

bool XboxController::GetStartButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kStart));
}
