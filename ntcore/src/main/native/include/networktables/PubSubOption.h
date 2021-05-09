// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

namespace nt {

struct PubSubOption {
  enum Type { kTypeImmediate, kTypePeriodic, kTypeLogging };

  static const PubSubOption kImmediate;

  static constexpr PubSubOption Periodic(double time) {
    return PubSubOption{kTypePeriodic, time};
  }

  static const PubSubOption kLogging;
  static const PubSubOption kDisableLogging;

  Type type;
  double value;

 private:
  constexpr PubSubOption(Type type, double value) : type{type}, value{value} {}
};

constexpr PubSubOption PubSubOption::kImmediate{kTypeImmediate, 1};
constexpr PubSubOption PubSubOption::kLogging{kTypeLogging, 1};
constexpr PubSubOption PubSubOption::kDisableLogging{kTypeLogging, 0};

using PublishOption = PubSubOption;
using SubscribeOption = PubSubOption;

}  // namespace nt
