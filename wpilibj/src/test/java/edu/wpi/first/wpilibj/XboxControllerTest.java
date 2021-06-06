// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import edu.wpi.first.hal.HAL;
import edu.wpi.first.wpilibj.simulation.XboxControllerSim;
import org.junit.jupiter.api.Test;

class XboxControllerTest {
  @Test
  void testGetX() {
    HAL.initialize(500, 0);
    XboxController joy = new XboxController(2);
    XboxControllerSim joysim = new XboxControllerSim(joy);

    joysim.setLeftX(0.35);
    joysim.setRightX(0.45);
    joysim.notifyNewData();
    assertEquals(0.35, joy.getLeftX(), 0.001);
    assertEquals(0.45, joy.getRightX(), 0.001);
  }

  @Test
  void testGetBumper() {
    HAL.initialize(500, 0);
    XboxController joy = new XboxController(2);
    XboxControllerSim joysim = new XboxControllerSim(joy);

    joysim.setLeftBumper(false);
    joysim.setRightBumper(true);
    joysim.notifyNewData();
    assertFalse(joy.getLeftBumper());
    assertTrue(joy.getRightBumper());
    // need to call pressed and released to clear flags
    joy.getLeftBumperPressed();
    joy.getLeftBumperReleased();
    joy.getRightBumperPressed();
    joy.getRightBumperReleased();

    joysim.setLeftBumper(true);
    joysim.setRightBumper(false);
    joysim.notifyNewData();
    assertTrue(joy.getLeftBumper());
    assertTrue(joy.getLeftBumperPressed());
    assertFalse(joy.getLeftBumperReleased());
    assertFalse(joy.getRightBumper());
    assertFalse(joy.getRightBumperPressed());
    assertTrue(joy.getRightBumperReleased());
  }

  @Test
  void testGetAButton() {
    HAL.initialize(500, 0);
    XboxController joy = new XboxController(2);
    XboxControllerSim joysim = new XboxControllerSim(joy);

    joysim.setAButton(false);
    joysim.notifyNewData();
    assertFalse(joy.getAButton());
    // need to call pressed and released to clear flags
    joy.getAButtonPressed();
    joy.getAButtonReleased();

    joysim.setAButton(true);
    joysim.notifyNewData();
    assertTrue(joy.getAButton());
    assertTrue(joy.getAButtonPressed());
    assertFalse(joy.getAButtonReleased());

    joysim.setAButton(false);
    joysim.notifyNewData();
    assertFalse(joy.getAButton());
    assertFalse(joy.getAButtonPressed());
    assertTrue(joy.getAButtonReleased());
  }
}
