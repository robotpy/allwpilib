/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.smartdashboard;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableInstance;
import edu.wpi.first.wpilibj.UtilityClassTest;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

class SmartDashboardTest extends UtilityClassTest<SmartDashboard> {
  private final NetworkTable m_table = NetworkTableInstance.getDefault().getTable("SmartDashboard");

  SmartDashboardTest() {
    super(SmartDashboard.class);
  }

  @BeforeEach
  void beforeEach() {
    m_table.getKeys().forEach(m_table::delete);
  }

  @Test
  void getBadValueTest() {
    assertEquals("Expected", SmartDashboard.getString("KEY_SHOULD_NOT_BE_FOUND", "Expected"));
  }

  @Test
  void putStringTest() {
    final String key = "putString";
    final String value = "thisIsAValue";

    SmartDashboard.putString(key, value);

    assertEquals(value, m_table.getEntry(key).getString(""));
  }

  @Test
  void getStringTest() {
    final String key = "getString";
    final String value = "thisIsAValue";

    m_table.getEntry(key).setString(value);

    assertEquals(value, SmartDashboard.getString(key, ""));
  }

  @Test
  void putNumberTest() {
    final String key = "PutNumber";
    final int value = 2147483647;

    SmartDashboard.putNumber(key, value);

    assertEquals(value, m_table.getEntry(key).getNumber(0).intValue());
  }

  @Test
  void getNumberTest() {
    final String key = "GetNumber";
    final int value = 2147483647;

    m_table.getEntry(key).setNumber(value);

    assertEquals(value, SmartDashboard.getNumber(key, 0), 0.01);
  }

  @Test
  void putBooleanTest() {
    final String key = "PutBoolean";
    final boolean value = true;

    SmartDashboard.putBoolean(key, value);

    assertEquals(value, m_table.getEntry(key).getBoolean(!value));
  }

  @Test
  void getBooleanTest() {
    final String key = "GetBoolean";
    final boolean value = true;

    m_table.getEntry(key).setBoolean(value);

    assertEquals(value, SmartDashboard.getBoolean(key, !value));
  }

  @Test
  void testReplaceString() {
    final String key = "testReplaceString";
    final String valueNew = "newValue";

    m_table.getEntry(key).setString("oldValue");
    SmartDashboard.putString(key, valueNew);

    assertEquals(valueNew, m_table.getEntry(key).getString(""));
  }

  @Test
  void putStringNullKeyTest() {
    assertThrows(NullPointerException.class,
        () -> SmartDashboard.putString(null, "This should not work"));
  }

  @Test
  void putStringNullValueTest() {
    assertThrows(NullPointerException.class,
        () -> SmartDashboard.putString("KEY_SHOULD_NOT_BE_STORED", null));
  }
}
