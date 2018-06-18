/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "LowFiSim/MotorModel/MotorModel.h"
#include "LowFiSim/MotorSim.h"
#include "Simulation/PWMSim.h"

namespace frc {
namespace sim {
namespace lowfi {

class WpiMotorSim : public MotorSim {
 public:
  explicit WpiMotorSim(int index, MotorModel& motorModelSimulator);

  void Update(double elapsedTime);
  double GetPosition() const override;
  double GetVelocity() const override;
  double GetAcceleration() const override;

 private:
  MotorModel& m_motorModelSimulation;
  frc::sim::PWMSim m_pwmSimulator;

  static constexpr double kDefaultVoltage = 12.0;
};

}  // namespace lowfi
}  // namespace sim
}  // namespace frc
