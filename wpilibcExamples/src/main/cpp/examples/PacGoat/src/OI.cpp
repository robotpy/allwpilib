/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "OI.h"

#include "Commands/Collect.h"
#include "Commands/DriveForward.h"
#include "Commands/LowGoal.h"
#include "Commands/SetCollectionSpeed.h"
#include "Commands/SetPivotSetpoint.h"
#include "Commands/Shoot.h"
#include "Subsystems/Collector.h"
#include "Subsystems/Pivot.h"

OI::OI() {
	m_r1.WhenPressed(new LowGoal());
	m_r2.WhenPressed(new Collect());

	m_l1.WhenPressed(new SetPivotSetpoint(Pivot::kShoot));
	m_l2.WhenPressed(new SetPivotSetpoint(Pivot::kShootNear));

	m_sticks.WhenActive(new Shoot());

	// SmartDashboard Buttons
	frc::SmartDashboard::PutData("Drive Forward", new DriveForward(2.25));
	frc::SmartDashboard::PutData("Drive Backward", new DriveForward(-2.25));
	frc::SmartDashboard::PutData("Start Rollers",
			new SetCollectionSpeed(Collector::kForward));
	frc::SmartDashboard::PutData("Stop Rollers",
			new SetCollectionSpeed(Collector::kStop));
	frc::SmartDashboard::PutData("Reverse Rollers",
			new SetCollectionSpeed(Collector::kReverse));
}

frc::Joystick& OI::GetJoystick() {
	return m_joystick;
}
