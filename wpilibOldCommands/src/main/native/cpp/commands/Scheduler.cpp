// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/commands/Scheduler.h"

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include <hal/FRCUsageReporting.h>
#include <networktables/NTSendableBuilder.h>
#include <networktables/NetworkTableEntry.h>
#include <wpi/mutex.h>
#include <wpi/sendable/SendableRegistry.h>

#include "frc/Errors.h"
#include "frc/buttons/ButtonScheduler.h"
#include "frc/commands/Command.h"
#include "frc/commands/Subsystem.h"
#include "frc/livewindow/LiveWindow.h"

using namespace frc;

struct Scheduler::Impl {
  void Remove(Command* command);
  void ProcessCommandAddition(Command* command);

  using SubsystemSet = std::set<Subsystem*>;
  SubsystemSet subsystems;
  wpi::mutex buttonsMutex;
  using ButtonVector = std::vector<std::unique_ptr<ButtonScheduler>>;
  ButtonVector buttons;
  using CommandVector = std::vector<Command*>;
  wpi::mutex additionsMutex;
  CommandVector additions;
  using CommandSet = std::set<Command*>;
  CommandSet commands;
  bool adding = false;
  bool enabled = true;
  std::vector<std::string> commandsBuf;
  std::vector<double> idsBuf;
  bool runningCommandsChanged = false;
};

Scheduler* Scheduler::GetInstance() {
  static Scheduler instance;
  return &instance;
}

void Scheduler::AddCommand(Command* command) {
  std::scoped_lock lock(m_impl->additionsMutex);
  if (std::find(m_impl->additions.begin(), m_impl->additions.end(), command) !=
      m_impl->additions.end()) {
    return;
  }
  m_impl->additions.push_back(command);
}

void Scheduler::AddButton(ButtonScheduler* button) {
  std::scoped_lock lock(m_impl->buttonsMutex);
  m_impl->buttons.emplace_back(button);
}

void Scheduler::RegisterSubsystem(Subsystem* subsystem) {
  if (!subsystem) {
    throw FRC_MakeError(err::NullParameter, "{}", "subsystem");
  }
  m_impl->subsystems.insert(subsystem);
}

void Scheduler::Run() {
  // Get button input (going backwards preserves button priority)
  {
    if (!m_impl->enabled) {
      return;
    }

    std::scoped_lock lock(m_impl->buttonsMutex);
    for (auto& button : m_impl->buttons) {
      button->Execute();
    }
  }

  // Call every subsystem's periodic method
  for (auto& subsystem : m_impl->subsystems) {
    subsystem->Periodic();
  }

  m_impl->runningCommandsChanged = false;

  // Loop through the commands
  for (auto cmdIter = m_impl->commands.begin();
       cmdIter != m_impl->commands.end();) {
    Command* command = *cmdIter;
    // Increment before potentially removing to keep the iterator valid
    ++cmdIter;
    if (!command->Run()) {
      Remove(command);
      m_impl->runningCommandsChanged = true;
    }
  }

  // Add the new things
  {
    std::scoped_lock lock(m_impl->additionsMutex);
    for (auto& addition : m_impl->additions) {
      // Check to make sure no adding during adding
      if (m_impl->adding) {
        FRC_ReportError(warn::IncompatibleState, "{}",
                        "Can not start command from cancel method");
      } else {
        m_impl->ProcessCommandAddition(addition);
      }
    }
    m_impl->additions.clear();
  }

  // Add in the defaults
  for (auto& subsystem : m_impl->subsystems) {
    if (subsystem->GetCurrentCommand() == nullptr) {
      if (m_impl->adding) {
        FRC_ReportError(warn::IncompatibleState, "{}",
                        "Can not start command from cancel method");
      } else {
        m_impl->ProcessCommandAddition(subsystem->GetDefaultCommand());
      }
    }
    subsystem->ConfirmCommand();
  }
}

void Scheduler::Remove(Command* command) {
  if (!command) {
    throw FRC_MakeError(err::NullParameter, "{}", "command");
  }

  m_impl->Remove(command);
}

void Scheduler::RemoveAll() {
  while (m_impl->commands.size() > 0) {
    Remove(*m_impl->commands.begin());
  }
}

void Scheduler::ResetAll() {
  RemoveAll();
  m_impl->subsystems.clear();
  m_impl->buttons.clear();
  m_impl->additions.clear();
  m_impl->commands.clear();
}

void Scheduler::SetEnabled(bool enabled) {
  m_impl->enabled = enabled;
}

void Scheduler::InitSendable(nt::NTSendableBuilder& builder) {
  builder.SetSmartDashboardType("Scheduler");
  auto namesEntry = builder.GetEntry("Names");
  auto idsEntry = builder.GetEntry("Ids");
  auto cancelEntry = builder.GetEntry("Cancel");
  builder.SetUpdateTable([=] {
    // Get the list of possible commands to cancel
    auto new_toCancel = cancelEntry.GetValue();
    wpi::span<const double> toCancel;
    if (new_toCancel) {
      toCancel = new_toCancel->GetDoubleArray();
    }

    // Cancel commands whose cancel buttons were pressed on the SmartDashboard
    if (!toCancel.empty()) {
      for (auto& command : m_impl->commands) {
        for (const auto& canceled : toCancel) {
          if (command->GetID() == canceled) {
            command->Cancel();
          }
        }
      }
      nt::NetworkTableEntry(cancelEntry).SetDoubleArray({});
    }

    // Set the running commands
    if (m_impl->runningCommandsChanged) {
      m_impl->commandsBuf.resize(0);
      m_impl->idsBuf.resize(0);
      for (const auto& command : m_impl->commands) {
        m_impl->commandsBuf.emplace_back(
            wpi::SendableRegistry::GetName(command));
        m_impl->idsBuf.emplace_back(command->GetID());
      }
      nt::NetworkTableEntry(namesEntry).SetStringArray(m_impl->commandsBuf);
      nt::NetworkTableEntry(idsEntry).SetDoubleArray(m_impl->idsBuf);
    }
  });
}

Scheduler::Scheduler() : m_impl(new Impl) {
  HAL_Report(HALUsageReporting::kResourceType_Command,
             HALUsageReporting::kCommand_Scheduler);
  wpi::SendableRegistry::AddLW(this, "Scheduler");
  frc::LiveWindow::SetEnabledCallback([this] {
    this->SetEnabled(false);
    this->RemoveAll();
  });
  frc::LiveWindow::SetDisabledCallback([this] { this->SetEnabled(true); });
}

Scheduler::~Scheduler() {
  wpi::SendableRegistry::Remove(this);
  frc::LiveWindow::SetEnabledCallback(nullptr);
  frc::LiveWindow::SetDisabledCallback(nullptr);
}

void Scheduler::Impl::Remove(Command* command) {
  if (!commands.erase(command)) {
    return;
  }

  for (auto&& requirement : command->GetRequirements()) {
    requirement->SetCurrentCommand(nullptr);
  }

  command->Removed();
}

void Scheduler::Impl::ProcessCommandAddition(Command* command) {
  if (command == nullptr) {
    return;
  }

  // Only add if not already in
  auto found = commands.find(command);
  if (found == commands.end()) {
    // Check that the requirements can be had
    const auto& requirements = command->GetRequirements();
    for (const auto requirement : requirements) {
      if (requirement->GetCurrentCommand() != nullptr &&
          !requirement->GetCurrentCommand()->IsInterruptible()) {
        return;
      }
    }

    // Give it the requirements
    adding = true;
    for (auto&& requirement : requirements) {
      if (requirement->GetCurrentCommand() != nullptr) {
        requirement->GetCurrentCommand()->Cancel();
        Remove(requirement->GetCurrentCommand());
      }
      requirement->SetCurrentCommand(command);
    }
    adding = false;

    commands.insert(command);

    command->StartRunning();
    runningCommandsChanged = true;
  }
}
