/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.math.interpolation;

import org.junit.jupiter.api.Test;

import edu.wpi.first.wpilibj.geometry.Rotation2d;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class TimeInterpolatableBufferTest {
  @Test
  public void testBufferInterpolation() {
    TimeInterpolatableBuffer<Rotation2d> buffer = TimeInterpolatableBuffer.createBuffer(10);

    buffer.addSample(0, new Rotation2d());
    assertEquals(0, buffer.getSample(0).getRadians(), 0.001);
    buffer.addSample(1, new Rotation2d(1));
    assertEquals(0.5, buffer.getSample(0.5).getRadians(), 0.001);
    assertEquals(1.0, buffer.getSample(1.0).getRadians(), 0.001);
    buffer.addSample(3, new Rotation2d(2));
    assertEquals(1.5, buffer.getSample(2).getRadians(), 0.001);

    buffer.addSample(10.5, new Rotation2d(2));
    assertEquals(new Rotation2d(1), buffer.getSample(0));
  }
}
