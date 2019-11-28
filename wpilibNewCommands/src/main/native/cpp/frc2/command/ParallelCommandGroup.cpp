/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc2/command/ParallelCommandGroup.h"

using namespace frc2;

ParallelCommandGroup::ParallelCommandGroup(
    std::vector<std::unique_ptr<Command>>&& commands) {
  AddCommands(std::move(commands));
}

void ParallelCommandGroup::Initialize() {
  for (auto& commandRunning : m_commands) {
    commandRunning.first->Initialize();
    commandRunning.second = true;
  }
  isRunning = true;
}

void ParallelCommandGroup::Execute() {
  for (auto& commandRunning : m_commands) {
    if (!commandRunning.second) continue;
    commandRunning.first->Execute();
    if (commandRunning.first->IsFinished()) {
      commandRunning.first->End(false);
      commandRunning.second = false;
    }
  }
}

void ParallelCommandGroup::End(bool interrupted) {
  if (interrupted) {
    for (auto& commandRunning : m_commands) {
      if (commandRunning.second) {
        commandRunning.first->End(true);
      }
    }
  }
  isRunning = false;
}

bool ParallelCommandGroup::IsFinished() {
  for (auto& command : m_commands) {
    if (command.second) return false;
  }
  return true;
}

bool ParallelCommandGroup::RunsWhenDisabled() const {
  return m_runWhenDisabled;
}

void ParallelCommandGroup::AddCommands(
    std::vector<std::unique_ptr<Command>>&& commands) {
  for (auto&& command : commands) {
    if (!RequireUngrouped(*command)) return;
  }

  if (isRunning) {
    wpi_setWPIErrorWithContext(CommandIllegalUse,
                               "Commands cannot be added to a CommandGroup "
                               "while the group is running");
  }

  for (auto&& command : commands) {
    if (RequirementsDisjoint(this, command.get())) {
      command->SetGrouped(true);
      AddRequirements(command->GetRequirements());
      m_runWhenDisabled &= command->RunsWhenDisabled();
      m_commands.emplace_back(std::move(command), false);
    } else {
      wpi_setWPIErrorWithContext(CommandIllegalUse,
                                 "Multiple commands in a parallel group cannot "
                                 "require the same subsystems");
      return;
    }
  }
}
