/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.util;

import java.util.stream.Stream;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import static org.junit.jupiter.api.Assertions.assertAll;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.params.provider.Arguments.arguments;

class ColorTest {
  private static final double kEpsilon = 1e-3;

  void assertColorMatches(double red, double green, double blue, Color color) {
    assertAll(
        () -> assertEquals(red, color.red, kEpsilon),
        () -> assertEquals(green, color.green, kEpsilon),
        () -> assertEquals(blue, color.blue, kEpsilon)
    );
  }

  @ParameterizedTest
  @MethodSource("staticColorProvider")
  void staticColorTest(double red, double green, double blue, Color color) {
    assertColorMatches(red, green, blue, color);
  }

  @ParameterizedTest
  @MethodSource("staticColorProvider")
  void colorEqualsTest(double red, double green, double blue, Color color) {
    assertEquals(color, new Color(red, green, blue));
  }

  static Stream<Arguments> staticColorProvider() {
    return Stream.of(
      arguments(0.0823529412, 0.376470589, 0.7411764706, Color.kDenim),
      arguments(0.0, 0.4, 0.7019607844, Color.kFirstBlue),
      arguments(0.9294117648, 0.1098039216, 0.1411764706, Color.kFirstRed)
    );
  }
}
