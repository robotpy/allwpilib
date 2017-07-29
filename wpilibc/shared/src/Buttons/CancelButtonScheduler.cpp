/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2011-2017. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Buttons/CancelButtonScheduler.h"

#include "Buttons/Button.h"
#include "Commands/Command.h"

frc::CancelButtonScheduler::CancelButtonScheduler(bool last, Trigger* button,
                                                  Command* orders)
    : ButtonScheduler(last, button, orders) {
  pressedLast = m_button->Grab();
}

void frc::CancelButtonScheduler::Execute() {
  if (m_button->Grab()) {
    if (!pressedLast) {
      pressedLast = true;
      m_command->Cancel();
    }
  } else {
    pressedLast = false;
  }
}
