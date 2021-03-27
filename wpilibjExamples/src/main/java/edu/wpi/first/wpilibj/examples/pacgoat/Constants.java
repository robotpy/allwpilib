// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj.examples.pacgoat;

public class Constants {
  public static final int kPCMId = 1;
  public static final int kJoystickPort = 0;

  public static final class DriveConstants {
    public static final int kFLPort = 1;
    public static final int kFRPort = 2;
    public static final int kRLPort = 3;
    public static final int kRRPort = 4;

    public static final int[] kLEncoderPorts = {1, 2};
    public static final int[] kREncoderPorts = {3, 4};
    public static final double kDistancePerPulse = 0.0785398;

    public static final int kGyroPort = 0;
  }

  public static final class CollectorConstants {
    public static final double kForward = -1;
    public static final double kStop = 0;
    public static final double kReverse = 1;

    public static final int kMotorPort = 6;
    public static final int kBallDetectorPort = 10;
    public static final int kOpenDetectorPort = 6;

    public static final int kPistonSolenoidId = 1;
  }

  public static final class PivotConstants {
    public static final double kCollect = 105;
    public static final double kLowGoal = 90;
    public static final double kShoot = 45;
    public static final double kShootNear = 30;

    public static final int kMotorPort = 5;

    public static final int kUpperLimitSwitchPort = 13;
    public static final int kLowerLimitSwitchPort = 12;

    public static final int kPotentiometerPort = 1;

    public static final double kP = 7.0;
    public static final double kI = 0.0;
    public static final double kD = 8.0;
  }

  public static final class ShooterConstants {
    public static final int[] kPiston1Ids = {3, 4};
    public static final int[] kPiston2Ids = {5, 6};
    public static final int kLatchPistonId = 2;

    public static final int kPiston1ReedSwitchFrontPort = 9;
    public static final int kPiston1ReedSwitchBackPort = 11;

    public static final int kHotGoalSensorPort = 7;
  }

  public static final class PneumaticsConstants {
    public static final int kPressureSensorPort = 3;
    public static final double kMaxPressure = 2.55;
  }
}
