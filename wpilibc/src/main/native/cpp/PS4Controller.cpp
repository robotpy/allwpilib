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

#define BUTTON_DEF(NAME) \
bool PS4Controller::Get##NAME##Button() const {\
  return GetRawButton(static_cast<int>(Button::k##NAME));\
}\
\
bool PS4Controller::Get##NAME##ButtonPressed() {\
  return GetRawButtonPressed(static_cast<int>(Button::k##NAME));\
}\
\
bool PS4Controller::Get##NAME##ButtonReleased() {\
  return GetRawButtonReleased(static_cast<int>(Button::k##NAME));\
}

BUTTON_DEF(Square)
BUTTON_DEF(Cross)
BUTTON_DEF(Circle)
BUTTON_DEF(Triangle)
BUTTON_DEF(L1)
BUTTON_DEF(R1)
BUTTON_DEF(L2)
BUTTON_DEF(R2)
BUTTON_DEF(Share)
BUTTON_DEF(Options)
BUTTON_DEF(LeftStick)
BUTTON_DEF(RightStick)
BUTTON_DEF(PS)

#undef BUTTON_DEF
// to avoid ScreenButton naming, the Screen functions don't use the macro

bool PS4Controller::GetScreen() const {
  return GetRawButton(static_cast<int>(Button::kScreen));
}

bool PS4Controller::GetScreenPressed() {
  return GetRawButtonPressed(static_cast<int>(Button::kScreen));
}

bool PS4Controller::GetScreenReleased() {
  return GetRawButtonReleased(static_cast<int>(Button::kScreen));
}
