// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/PS4Controller.h"

#include <hal/FRCUsageReporting.h>

using namespace frc;

PS4Controller::PS4Controller(int port) : GenericHID(port) {
  HAL_Report(HALUsageReporting::kResourceType_PS4Controller, port + 1);
}

double PS4Controller::GetLeftX() const {
  return GetRawAxis(static_cast<int>(Axis::kLeftX));
}

double PS4Controller::GetRightX() const {
  return GetRawAxis(static_cast<int>(Axis::kRightX));
}

double PS4Controller::GetLeftY() const {
  return GetRawAxis(static_cast<int>(Axis::kLeftY));
}

double PS4Controller::GetRightY() const {
  return GetRawAxis(static_cast<int>(Axis::kRightY));
}

double PS4Controller::GetL2Axis() const {
  return GetRawAxis(static_cast<int>(Axis::kL2));
}

double PS4Controller::GetR2Axis() const {
  return GetRawAxis(static_cast<int>(Axis::kR2));
}

bool PS4Controller::GetSquareButton() const {
  return GetRawButton(static_cast<int>(Button::kSquare));
}

bool PS4Controller::GetSquareButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kSquare));
}

bool PS4Controller::GetSquareButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kSquare));
}

bool PS4Controller::GetCrossButton() const {
  return GetRawButton(static_cast<int>(Button::kCross));
}

bool PS4Controller::GetCrossButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kCross));
}

bool PS4Controller::GetCrossButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kCross));
}

bool PS4Controller::GetCircleButton() const {
  return GetRawButton(static_cast<int>(Button::kCircle));
}

bool PS4Controller::GetCircleButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kCircle));
}

bool PS4Controller::GetCircleButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kCircle));
}

bool PS4Controller::GetTriangleButton() const {
  return GetRawButton(static_cast<int>(Button::kTriangle));
}

bool PS4Controller::GetTriangleButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kTriangle));
}

bool PS4Controller::GetTriangleButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kTriangle));
}

bool PS4Controller::GetL1Button() const {
  return GetRawButton(static_cast<int>(Button::kL1));
}

bool PS4Controller::GetL1ButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kL1));
}

bool PS4Controller::GetL1ButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kL1));
}

bool PS4Controller::GetR1Button() const {
  return GetRawButton(static_cast<int>(Button::kR1));
}

bool PS4Controller::GetR1ButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kR1));
}

bool PS4Controller::GetR1ButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kR1));
}

bool PS4Controller::GetL2Button() const {
  return GetRawButton(static_cast<int>(Button::kL2));
}

bool PS4Controller::GetL2ButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kL2));
}

bool PS4Controller::GetL2ButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kL2));
}

bool PS4Controller::GetR2Button() const {
  return GetRawButton(static_cast<int>(Button::kR2));
}

bool PS4Controller::GetR2ButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kR2));
}

bool PS4Controller::GetR2ButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kR2));
}

bool PS4Controller::GetShareButton() const {
  return GetRawButton(static_cast<int>(Button::kShare));
}

bool PS4Controller::GetShareButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kShare));
}

bool PS4Controller::GetShareButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kShare));
}

bool PS4Controller::GetOptionsButton() const {
  return GetRawButton(static_cast<int>(Button::kOptions));
}

bool PS4Controller::GetOptionsButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kOptions));
}

bool PS4Controller::GetOptionsButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kOptions));
}

bool PS4Controller::GetLeftStickButton() const {
  return GetRawButton(static_cast<int>(Button::kLeftStick));
}

bool PS4Controller::GetLeftStickButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kLeftStick));
}

bool PS4Controller::GetLeftStickButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kLeftStick));
}

bool PS4Controller::GetRightStickButton() const {
  return GetRawButton(static_cast<int>(Button::kRightStick));
}

bool PS4Controller::GetRightStickButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kRightStick));
}

bool PS4Controller::GetRightStickButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kRightStick));
}

bool PS4Controller::GetPSButton() const {
  return GetRawButton(static_cast<int>(Button::kPS));
}

bool PS4Controller::GetPSButtonPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kPS));
}

bool PS4Controller::GetPSButtonReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kPS));
}

bool PS4Controller::GetScreen() const {
  return GetRawButton(static_cast<int>(Button::kScreen));
}

bool PS4Controller::GetScreenPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kScreen));
}

bool PS4Controller::GetScreenReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kScreen));
}
