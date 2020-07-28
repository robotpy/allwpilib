/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <frc/TimedRobot.h>

#include "frc/simulation/Mechanism2D.h"

class MyRobot : public frc::TimedRobot {
  /**
   * This function is run when the robot is first started up and should be
   * used for any initialization code.
   */
  void RobotInit() override {}

  /**
   * This function is run once each time the robot enters autonomous mode
   */
  void AutonomousInit() override {}

  /**
   * This function is called periodically during autonomous
   */
  void AutonomousPeriodic() override {}

  /**
   * This function is called once each time the robot enters tele-operated mode
   */
  void TeleopInit() override {}

  /**
   * This function is called periodically during operator control
   */
  void TeleopPeriodic() override {}

  /**
   * This function is called periodically during test mode
   */
  void TestPeriodic() override {}

  frc::Mechanism2D mechanism2D;
  int counter = 0;
  /**
   * This function is called periodically during all modes
   */
  void RobotPeriodic() override {
    mechanism2D.SetLigamentAngle("/JasonOne/one/", counter++);
    wpi::outs() << "MyRobot.cpp " << counter << "\n";
  }
};

int main() { return frc::StartRobot<MyRobot>(); }
