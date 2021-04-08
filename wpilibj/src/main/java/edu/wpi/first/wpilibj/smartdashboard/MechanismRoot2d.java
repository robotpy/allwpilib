// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj.smartdashboard;

import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableEntry;
import java.util.HashMap;
import java.util.Map;

/**
 * A Mechanism2d root node.
 *
 * @see Mechanism2d
 */
public final class MechanismRoot2d {
  private final String m_name;
  private NetworkTable m_table;
  private final Map<String, MechanismObject2d> m_objects = new HashMap<>(1);
  private double m_x;
  private NetworkTableEntry m_xEntry;
  private double m_y;
  private NetworkTableEntry m_yEntry;

  /**
   * Package-private constructor for roots.
   *
   * @param name name
   * @param x x coordinate of root (provide only when constructing a root node)
   * @param y y coordinate of root (provide only when constructing a root node)
   */
  MechanismRoot2d(String name, double x, double y) {
    m_name = name;
    m_x = x;
    m_y = y;
  }

  public final <T extends MechanismObject2d> T append(T object) {
    if (m_objects.containsKey(object.getName()))
      throw new UnsupportedOperationException("Mechanism object names must be unique!");
    m_objects.put(object.getName(), object);
    if (m_table != null) {
      object.update(m_table.getSubTable(object.getName()));
    }
    return object;
  }

  /**
   * Set the root's position.
   *
   * @param x new x coordinate
   * @param y new y coordinate
   */
  public void setPosition(double x, double y) {
    m_x = x;
    m_y = y;
  }

  final void update(NetworkTable table) {
    m_table = table;
    m_xEntry = m_table.getEntry("x");
    m_yEntry = m_table.getEntry("y");
    flush();
    for (MechanismObject2d obj : m_objects.values()) {
      obj.update(m_table.getSubTable(obj.getName()));
    }
  }

  public final String getName() {
    return m_name;
  }

  private void flush() {
    if (m_xEntry != null) {
      m_xEntry.setDouble(m_x);
    }
    if (m_yEntry != null) {
      m_yEntry.setDouble(m_y);
    }
  }

  @Override
  public boolean equals(Object obj) {
    return obj instanceof MechanismRoot2d && ((MechanismRoot2d) obj).m_name.equals(this.m_name);
  }
}
