/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "glass/hardware/Relay.h"

#include <imgui.h>

#include "glass/Context.h"
#include "glass/DataSource.h"
#include "glass/support/ExtraGuiWidgets.h"

using namespace glass;

void glass::DisplayRelay(RelayModel* model, int index, bool outputsEnabled) {
  auto forwardData = model->GetForwardData();
  auto reverseData = model->GetReverseData();

  if (!forwardData && !reverseData) {
    return;
  }

  bool forward = false;
  bool reverse = false;
  if (outputsEnabled) {
    if (forwardData) forward = forwardData->GetValue();
    if (reverseData) reverse = reverseData->GetValue();
  }

  std::string* name = GetStorage().GetStringRef("name");
  ImGui::PushID("name");
  if (!name->empty())
    ImGui::Text("%s [%d]", name->c_str(), index);
  else
    ImGui::Text("Relay[%d]", index);
  ImGui::PopID();
  if (PopupEditName("name", name)) {
    if (forwardData) forwardData->SetName(name->c_str());
    if (reverseData) reverseData->SetName(name->c_str());
  }
  ImGui::SameLine();

  // show forward and reverse as LED indicators
  static const ImU32 colors[] = {IM_COL32(255, 255, 102, 255),
                                 IM_COL32(255, 0, 0, 255),
                                 IM_COL32(128, 128, 128, 255)};
  int values[2] = {reverseData ? (reverse ? 2 : -2) : -3,
                   forwardData ? (forward ? 1 : -1) : -3};
  DataSource* sources[2] = {reverseData, forwardData};
  DrawLEDSources(values, sources, 2, 2, colors);
}

void glass::DisplayRelays(RelaysModel* model, bool outputsEnabled,
                          wpi::StringRef noneMsg) {
  bool hasAny = false;
  bool first = true;
  model->ForEachRelay([&](RelayModel& relay, int i) {
    hasAny = true;

    if (!first)
      ImGui::Separator();
    else
      first = false;

    PushID(i);
    DisplayRelay(&relay, i, outputsEnabled);
    PopID();
  });
  if (!hasAny && !noneMsg.empty())
    ImGui::TextUnformatted(noneMsg.begin(), noneMsg.end());
}
