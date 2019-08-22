/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "subsystems/Claw.h"
#include "subsystems/Wrist.h"
#include "subsystems/Elevator.h"

#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/CommandHelper.h>

/**
 * Pickup a soda can (if one is between the open claws) and
 * get it in a safe state to drive around.
 */
class Pickup : public frc2::CommandHelper<frc2::SequentialCommandGroup, Pickup> {
 public:
  Pickup(Claw* claw, Wrist* wrist, Elevator* elevator);
};
