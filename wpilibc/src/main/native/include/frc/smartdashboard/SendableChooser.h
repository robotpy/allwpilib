/*----------------------------------------------------------------------------*/
/* Copyright (c) 2011-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <memory>

#include <wpi/StringMap.h>
#include <wpi/StringRef.h>
#include <wpi/deprecated.h>

#include "frc/smartdashboard/SendableBuilder.h"
#include "frc/smartdashboard/SendableChooserBase.h"

namespace frc {

/**
 * The SendableChooser class is a useful tool for presenting a selection of
 * options to the SmartDashboard.
 *
 * For instance, you may wish to be able to select between multiple autonomous
 * modes. You can do this by putting every possible Command you want to run as
 * an autonomous into a SendableChooser and then put it into the SmartDashboard
 * to have a list of options appear on the laptop. Once autonomous starts,
 * simply ask the SendableChooser what the selected value is.
 *
 * @tparam T The type of values to be stored
 * @see SmartDashboard
 */
template <class T>
class SendableChooser : public SendableChooserBase {
  wpi::StringMap<T> m_choices;

  template <class U>
  static U _unwrap_smart_ptr(const U& value);

  template <class U>
  static U* _unwrap_smart_ptr(const std::unique_ptr<U>& value);

  template <class U>
  static std::weak_ptr<U> _unwrap_smart_ptr(const std::shared_ptr<U>& value);

 public:
  ~SendableChooser() override = default;
  SendableChooser(SendableChooser&& rhs) = default;
  SendableChooser& operator=(SendableChooser&& rhs) = default;

  void AddOption(wpi::StringRef name, T object);
  void SetDefaultOption(wpi::StringRef name, T object);

  /**
   * Adds the given object to the list of options.
   *
   * On the SmartDashboard on the desktop, the object will appear as the given
   * name.
   *
   * @deprecated use AddOption(wpi::StringRef name, T object) instead.
   *
   * @param name   the name of the option
   * @param object the option
   */
  WPI_DEPRECATED("use AddOption() instead")
  void AddObject(wpi::StringRef name, T object) { AddOption(name, object); }

  /**
   * Add the given object to the list of options and marks it as the default.
   *
   * Functionally, this is very close to AddOption() except that it will use
   * this as the default option if none other is explicitly selected.
   *
   * @deprecated use SetDefaultOption(wpi::StringRef name, T object) instead.
   *
   * @param name   the name of the option
   * @param object the option
   */
  WPI_DEPRECATED("use SetDefaultOption() instead")
  void AddDefault(wpi::StringRef name, T object) {
    SetDefaultOption(name, object);
  }

  auto GetSelected() -> decltype(_unwrap_smart_ptr(m_choices[""]));

  void InitSendable(SendableBuilder& builder) override;
};

}  // namespace frc

#include "frc/smartdashboard/SendableChooser.inc"
