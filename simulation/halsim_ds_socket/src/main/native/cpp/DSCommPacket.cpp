/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "DSCommPacket.h"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include <mockdata/DriverStationData.h>
#include <mockdata/MockHooks.h>
#include <wpi/ArrayRef.h>
#include <wpi/Format.h>

using namespace halsim;

DSCommPacket::DSCommPacket() {
  for (auto& i : m_joystick_packets) {
    i.ResetTcp();
    i.ResetUdp();
  }
}

/*----------------------------------------------------------------------------
**  The following methods help parse and hold information about the
**   driver station and it's joysticks.
**--------------------------------------------------------------------------*/

void DSCommPacket::SetControl(uint8_t control, uint8_t request) {
  std::memset(&m_control_word, 0, sizeof(m_control_word));
  m_control_word.enabled = (control & kEnabled) != 0;
  m_control_word.autonomous = (control & kAutonomous) != 0;
  m_control_word.test = (control & kTest) != 0;
  m_control_word.eStop = (control & kEmergencyStop) != 0;
  m_control_word.fmsAttached = (control & kFMS_Attached) != 0;
  m_control_word.dsAttached = (request & kRequestNormalMask) != 0;

  m_control_sent = control;
}

void DSCommPacket::SetAlliance(uint8_t station_code) {
  m_alliance_station = static_cast<HAL_AllianceStationID>(station_code);
}

void DSCommPacket::ReadMatchtimeTag(wpi::ArrayRef<uint8_t> tagData) {
  if (tagData.size() < 6) return;

  uint32_t store = tagData[2] << 24;
  store |= tagData[3] << 16;
  store |= tagData[4] << 8;
  store |= tagData[5];

  float matchTime = *reinterpret_cast<float*>(&store);
  m_match_time = matchTime;
}

static int GetBytesForBits(int bits) {
  if (bits == 0) {
    return 0;
  } else if (bits <= 8) {
    return 1;
  } else if (bits <= 16) {
    return 2;
  } else if (bits <= 24) {
    return 3;
  } else if (bits <= 32) {
    return 4;
  } else if (bits <= 40) {
    return 5;
  } else if (bits <= 48) {
    return 6;
  } else if (bits <= 56) {
    return 7;
  } else {
    return 8;
  }
}

void DSCommPacket::ReadJoystickTag(wpi::ArrayRef<uint8_t> dataInput,
                                   int index) {
  DSCommJoystickPacket& stick = m_joystick_packets[index];
  stick.ResetUdp();

  if (dataInput.size() == 2) {
    return;
  }

  dataInput = dataInput.slice(2);

  // Read axes
  int axesLength = dataInput[0];
  for (int i = 0; i < axesLength; i++) {
    int8_t value = dataInput[1 + i];
    if (value < 0) {
      stick.axes.axes[i] = value / 128.0;
    } else {
      stick.axes.axes[i] = value / 127.0;
    }
  }
  stick.axes.count = axesLength;

  dataInput = dataInput.slice(1 + axesLength);

  // Read Buttons
  int buttonCount = dataInput[0];
  int numBytes = GetBytesForBits(buttonCount);
  stick.buttons.buttons = 0;
  for (int i = 0; i < numBytes; i++) {
    stick.buttons.buttons |= dataInput[1 + i] << (8 * (i));
  }
  stick.buttons.count = buttonCount;

  dataInput = dataInput.slice(1 + numBytes);

  int povsLength = dataInput[0];
  for (int i = 0; i < povsLength * 2; i += 2) {
    stick.povs.povs[i] = (dataInput[1 + i] << 8) | dataInput[2 + i];
  }

  stick.povs.count = povsLength;

  return;
}

/*----------------------------------------------------------------------------
**  Communication methods
**--------------------------------------------------------------------------*/
void DSCommPacket::DecodeTCP(wpi::ArrayRef<uint8_t> packet) {
  // No header
  while (!packet.empty()) {
    int tagLength = packet[0] << 8 | packet[1];
    auto tagPacket = packet.slice(0, tagLength + 2);

    if (tagLength == 0) {
      return;
    }

    switch (packet[2]) {
      case kJoystickNameTag:
        ReadJoystickDescriptionTag(tagPacket);
        break;
      case kGameDataTag:
        ReadGameSpecificMessageTag(tagPacket);
        break;
      case kMatchInfoTag:
        ReadNewMatchInfoTag(tagPacket);
        break;
    }
    packet = packet.slice(tagLength + 2);
  }
}

void DSCommPacket::DecodeUDP(wpi::ArrayRef<uint8_t> packet) {
  if (packet.size() < 6) return;
  // Decode fixed header
  m_hi = packet[0];
  m_lo = packet[1];
  // Comm Version is packet 2, ignore
  SetControl(packet[3], packet[4]);
  SetAlliance(packet[5]);

  // Return if packet finished
  if (packet.size() == 6) return;

  // Else, handle tagged data
  packet = packet.slice(6);

  int joystickNum = 0;

  // Loop to handle multiple tags
  while (!packet.empty()) {
    auto tagLength = packet[0];
    auto tagPacket = packet.slice(0, tagLength + 1);

    switch (packet[1]) {
      case kJoystickDataTag:
        ReadJoystickTag(tagPacket, joystickNum);
        joystickNum++;
        break;
      case kMatchTimeTag:
        ReadMatchtimeTag(tagPacket);
        break;
    }
    packet = packet.slice(tagLength + 1);
  }
}

void DSCommPacket::ReadNewMatchInfoTag(wpi::ArrayRef<uint8_t> data) {
  // Size 2 bytes, tag 1 byte
  if (data.size() <= 3) return;

  int nameLength = std::min<size_t>(data[3], sizeof(matchInfo.eventName) - 1);

  for (int i = 0; i < nameLength; i++) {
    matchInfo.eventName[i] = data[4 + i];
  }

  matchInfo.eventName[nameLength] = '\0';

  data = data.slice(4 + nameLength);

  if (data.size() < 4) return;

  matchInfo.matchType = static_cast<HAL_MatchType>(
      data[0]);  // None, Practice, Qualification, Elimination, Test
  matchInfo.matchNumber = (data[1] << 8) | data[2];
  matchInfo.replayNumber = data[3];

  HALSIM_SetMatchInfo(&matchInfo);
}

void DSCommPacket::ReadGameSpecificMessageTag(wpi::ArrayRef<uint8_t> data) {
  // Size 2 bytes, tag 1 byte
  if (data.size() <= 3) return;

  int length = std::min<size_t>(((data[0] << 8) | data[1]) - 1,
                                sizeof(matchInfo.gameSpecificMessage));
  for (int i = 0; i < length; i++) {
    matchInfo.gameSpecificMessage[i] = data[3 + i];
  }

  matchInfo.gameSpecificMessageSize = length;

  HALSIM_SetMatchInfo(&matchInfo);
}
void DSCommPacket::ReadJoystickDescriptionTag(wpi::ArrayRef<uint8_t> data) {
  if (data.size() < 3) return;
  data = data.slice(3);
  int joystickNum = data[0];
  DSCommJoystickPacket& packet = m_joystick_packets[joystickNum];
  packet.ResetTcp();
  packet.descriptor.isXbox = data[1] != 0 ? 1 : 0;
  packet.descriptor.type = data[2];
  int nameLength =
      std::min<size_t>(data[3], (sizeof(packet.descriptor.name) - 1));
  for (int i = 0; i < nameLength; i++) {
    packet.descriptor.name[i] = data[4 + i];
  }
  data = data.slice(4 + nameLength);
  packet.descriptor.name[nameLength] = '\0';
  int axesCount = data[0];
  packet.descriptor.axisCount = axesCount;
  for (int i = 0; i < axesCount; i++) {
    packet.descriptor.axisTypes[i] = data[1 + i];
  }
  data = data.slice(1 + axesCount);

  packet.descriptor.buttonCount = data[0];
  packet.descriptor.povCount = data[1];
}

void DSCommPacket::SendJoysticks(void) {
  for (int i = 0; i < HAL_kMaxJoysticks; i++) {
    DSCommJoystickPacket& packet = m_joystick_packets[i];
    HALSIM_SetJoystickAxes(i, &packet.axes);
    HALSIM_SetJoystickPOVs(i, &packet.povs);
    HALSIM_SetJoystickButtons(i, &packet.buttons);
    HALSIM_SetJoystickDescriptor(i, &packet.descriptor);
  }
}

void DSCommPacket::SetupSendBuffer(wpi::raw_uv_ostream& buf) {
  SetupSendHeader(buf);
  SetupJoystickTag(buf);
}

void DSCommPacket::SetupSendHeader(wpi::raw_uv_ostream& buf) {
  static constexpr uint8_t kCommVersion = 0x01;

  // High low packet index, comm version
  buf << m_hi << m_lo << kCommVersion;

  // Control word and status check
  buf << m_control_sent
      << static_cast<uint8_t>(HALSIM_GetProgramStarted() ? kRobotHasCode : 0);

  // Battery voltage high and low
  buf << static_cast<uint8_t>(12) << static_cast<uint8_t>(0);

  // Request (Always 0)
  buf << static_cast<uint8_t>(0);
}

void DSCommPacket::SetupJoystickTag(wpi::raw_uv_ostream& buf) {
  static constexpr uint8_t kHIDTag = 0x01;

  // HID tags are sent 1 per device
  int64_t outputs;
  int32_t rightRumble;
  int32_t leftRumble;
  for (int i = 0; i < m_joystick_packets.size(); i++) {
    // Length is 9, 1 tag and 8 data.
    buf << static_cast<uint8_t>(9) << kHIDTag;
    HALSIM_GetJoystickOutputs(i, &outputs, &leftRumble, &rightRumble);
    auto op = static_cast<uint32_t>(outputs);
    auto rr = static_cast<uint16_t>(rightRumble);
    auto lr = static_cast<uint16_t>(leftRumble);
    buf.write((op >> 24 & 0xFF));
    buf.write((op >> 16 & 0xFF));
    buf.write((op >> 8 & 0xFF));
    buf.write((op & 0xFF));
    buf.write((rr >> 8 & 0xFF));
    buf.write((rr & 0xFF));
    buf.write((lr >> 8 & 0xFF));
    buf.write((lr & 0xFF));
  }
}

void DSCommPacket::SendUDPToHALSim(void) {
  SendJoysticks();

  HALSIM_SetDriverStationMatchTime(m_match_time);
  HALSIM_SetDriverStationEnabled(m_control_word.enabled);
  HALSIM_SetDriverStationAutonomous(m_control_word.autonomous);
  HALSIM_SetDriverStationTest(m_control_word.test);
  HALSIM_SetDriverStationEStop(m_control_word.eStop);
  HALSIM_SetDriverStationFmsAttached(m_control_word.fmsAttached);
  HALSIM_SetDriverStationDsAttached(m_control_word.dsAttached);
  HALSIM_SetDriverStationAllianceStationId(m_alliance_station);

  HALSIM_NotifyDriverStationNewData();
}
