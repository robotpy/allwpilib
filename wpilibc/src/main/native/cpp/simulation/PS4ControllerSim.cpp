// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/simulation/PS4ControllerSim.h"

#include "frc/PS4Controller.h"

using namespace frc;
using namespace frc::sim;

PS4ControllerSim::PS4ControllerSim(const PS4Controller& joystick)
    : GenericHIDSim{joystick} {
  SetAxisCount(6);
  SetButtonCount(10);
}

PS4ControllerSim::PS4ControllerSim(int port) : GenericHIDSim{port} {
  SetAxisCount(6);
  SetButtonCount(10);
}

void PS4ControllerSim::SetLeftX(double value) {
  SetRawAxis(static_cast<int>(PS4Controller::Axis::kLeftX), value);
}

void PS4ControllerSim::SetRightX(double value) {
  SetRawAxis(static_cast<int>(PS4Controller::Axis::kRightX), value);
}

void PS4ControllerSim::SetLeftY(double value) {
  SetRawAxis(static_cast<int>(PS4Controller::Axis::kLeftY), value);
}

void PS4ControllerSim::SetRightY(double value) {
  SetRawAxis(static_cast<int>(PS4Controller::Axis::kRightY), value);
}

void PS4ControllerSim::SetL2Axis(double value) {
  SetRawAxis(static_cast<int>(PS4Controller::Axis::kL2), value);
}

void PS4ControllerSim::SetR2Axis(double value) {
  SetRawAxis(static_cast<int>(PS4Controller::Axis::kR2), value);
}

#define SIM_BUTTON_SETTER(NAME)\
void PS4ControllerSim::Set##NAME##Button(bool value) {\
  SetRawButton(static_cast<int>(PS4Controller::Button::k##NAME), value);\
}

SIM_BUTTON_SETTER(Square)
SIM_BUTTON_SETTER(Cross)
SIM_BUTTON_SETTER(Circle)
SIM_BUTTON_SETTER(Triangle)
SIM_BUTTON_SETTER(L1)
SIM_BUTTON_SETTER(R1)
SIM_BUTTON_SETTER(L2)
SIM_BUTTON_SETTER(R2)
SIM_BUTTON_SETTER(Share)
SIM_BUTTON_SETTER(Options)
SIM_BUTTON_SETTER(LeftStick)
SIM_BUTTON_SETTER(RightStick)
SIM_BUTTON_SETTER(PS)

#undef SIM_BUTTON_SETTER