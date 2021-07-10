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
  SetRawAxis(PS4Controller::Axis::kLeftX, value);
}

void PS4ControllerSim::SetRightX(double value) {
  SetRawAxis(PS4Controller::Axis::kRightX, value);
}

void PS4ControllerSim::SetLeftY(double value) {
  SetRawAxis(PS4Controller::Axis::kLeftY, value);
}

void PS4ControllerSim::SetRightY(double value) {
  SetRawAxis(PS4Controller::Axis::kRightY, value);
}

void PS4ControllerSim::SetL2Axis(double value) {
  SetRawAxis(PS4Controller::Axis::kL2, value);
}

void PS4ControllerSim::SetR2Axis(double value) {
  SetRawAxis(PS4Controller::Axis::kR2, value);
}

void PS4ControllerSim::SetSquareButton(bool value) {
  SetRawButton(PS4Controller::Button::kSquare, value);
}

void PS4ControllerSim::SetCrossButton(bool value) {
  SetRawButton(PS4Controller::Button::kCross, value);
}

void PS4ControllerSim::SetCircleButton(bool value) {
  SetRawButton(PS4Controller::Button::kCircle, value);
}

void PS4ControllerSim::SetTriangleButton(bool value) {
  SetRawButton(PS4Controller::Button::kTriangle, value);
}

void PS4ControllerSim::SetL1Button(bool value) {
  SetRawButton(PS4Controller::Button::kL1, value);
}

void PS4ControllerSim::SetR1Button(bool value) {
  SetRawButton(PS4Controller::Button::kR1, value);
}

void PS4ControllerSim::SetL2Button(bool value) {
  SetRawButton(PS4Controller::Button::kL2, value);
}

void PS4ControllerSim::SetR2Button(bool value) {
  SetRawButton(PS4Controller::Button::kR2, value);
}

void PS4ControllerSim::SetShareButton(bool value) {
  SetRawButton(PS4Controller::Button::kShare, value);
}

void PS4ControllerSim::SetOptionsButton(bool value) {
  SetRawButton(PS4Controller::Button::kOptions, value);
}

void PS4ControllerSim::SetLeftStickButton(bool value) {
  SetRawButton(PS4Controller::Button::kLeftStick, value);
}

void PS4ControllerSim::SetRightStickButton(bool value) {
  SetRawButton(PS4Controller::Button::kRightStick, value);
}

void PS4ControllerSim::SetPSButton(bool value) {
  SetRawButton(PS4Controller::Button::kPS, value);
}
