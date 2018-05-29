/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "WpiSimulators/WpiMotorSimulator.h"

namespace frc {
namespace sim {
namespace lowfi {

WpiMotorSimulator::WpiMotorSimulator(int index) : m_pwmSimulator(index) {}

WpiMotorSimulator::~WpiMotorSimulator() {}

void WpiMotorSimulator::SetMotorModelSimulation(
    const std::shared_ptr<MotorModelSimulation>& motorModelSimulator) {
  m_motorModelSimulation = motorModelSimulator;
}

void WpiMotorSimulator::Update(double elapsedTime) {
  m_motorModelSimulation->SetVoltage(m_pwmSimulator.GetSpeed() *
                                     kDefaultVoltage);
  m_motorModelSimulation->Update(elapsedTime);
}

double WpiMotorSimulator::GetPosition() const {
  return m_motorModelSimulation->GetPosition();
}

double WpiMotorSimulator::GetVelocity() const {
  return m_motorModelSimulation->GetVelocity();
}

double WpiMotorSimulator::GetAcceleration() const {
  return m_motorModelSimulation->GetAcceleration();
}

}  // namespace lowfi
}  // namespace sim
}  // namespace frc
