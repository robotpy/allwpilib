// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <memory>
#include <string>

#include <networktables/NetworkTableEntry.h>
#include <wpi/StringMap.h>
#include <wpi/mutex.h>

#include "MechanismRoot2d.h"
#include "frc/smartdashboard/Sendable.h"
#include "frc/smartdashboard/SendableHelper.h"
#include "frc/util/Color8Bit.h"

namespace frc {

/**
 * Visual 2D representation of arms, elevators, and general mechanisms; through
 * a node-based API.
 *
 * A Mechanism2d object is published and contains at least one root node. Other
 * nodes (such as ligaments) are recursively based on other nodes.
 *
 * Except for the Mechanism2d container object, none of the objects should be
 * passed or interacted with by value! Obtain pointers from factory methods such
 * as Mechanism2d.GetRoot() and MechanismObject2d.Append<>(). The Mechanism2d
 * container object owns the root nodes, and each node internally owns the nodes
 * based on it. Beware not to let the Mechanism2d object out of scope - all
 * nodes will be recursively destructed!
 *
 * @see MechanismObject2d
 * @see MechanismLigament2d
 * @see MechanismRoot2d
 */
class Mechanism2d : public Sendable, public SendableHelper<Mechanism2d> {
 public:
  /**
   * Create a new Mechanism2d window with the given dimensions.
   *
   * @param width the window width
   * @param height the window height
   */
  Mechanism2d(double width, double height);

  /**
   * Get or create a root in this Mechanism2d window with the given name and
   * position.
   *
   * <p>If a root with the given name already exists, the given x and y
   * coordinates are not used.
   *
   * @param name the root name
   * @param x the root x coordinate
   * @param y the root y coordinate
   * @return a new root object, or the existing one with the given name.
   */
  MechanismRoot2d* GetRoot(wpi::StringRef name, double x, double y);

  /**
   * Set the Mechanism2d window background color.
   *
   * @param color the new background color
   */
  void SetBackgroundColor(const Color8Bit& color);

  void InitSendable(SendableBuilder& builder) override;

 private:
  double m_width;
  double m_height;
  std::string m_color;
  mutable wpi::mutex m_mutex;
  std::shared_ptr<nt::NetworkTable> m_table;
  wpi::StringMap<std::unique_ptr<MechanismRoot2d>> m_roots;
};
}  // namespace frc
