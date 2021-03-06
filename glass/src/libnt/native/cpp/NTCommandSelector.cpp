// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "glass/networktables/NTCommandSelector.h"

#include <fmt/format.h>
#include <wpi/StringExtras.h>

using namespace glass;

NTCommandSelectorModel::NTCommandSelectorModel(std::string_view path)
    : NTCommandSelectorModel(nt::GetDefaultInstance(), path) {}

NTCommandSelectorModel::NTCommandSelectorModel(NT_Inst instance,
                                               std::string_view path)
    : m_nt(instance),
      m_running(m_nt.GetEntry(fmt::format("{}/running", path))),
      m_name(m_nt.GetEntry(fmt::format("{}/.name", path))),
      m_runningData(fmt::format("NTCmd:{}", path)),
      m_nameValue(wpi::rsplit(path, '/').second) {
  m_runningData.SetDigital(true);
  m_nt.AddListener(m_running);
  m_nt.AddListener(m_name);
}

void NTCommandSelectorModel::SetRunning(bool run) {
  nt::SetEntryValue(m_running, nt::NetworkTableValue::MakeBoolean(run));
}

void NTCommandSelectorModel::Update() {
  for (auto&& event : m_nt.PollListener()) {
    if (event.entry == m_running) {
      if (event.value && event.value->IsBoolean()) {
        m_runningData.SetValue(event.value->GetBoolean());
      }
    } else if (event.entry == m_name) {
      if (event.value && event.value->IsString()) {
        m_nameValue = event.value->GetString();
      }
    }
  }
}

bool NTCommandSelectorModel::Exists() {
  return m_nt.IsConnected() && nt::GetEntryType(m_running) != NT_UNASSIGNED;
}
