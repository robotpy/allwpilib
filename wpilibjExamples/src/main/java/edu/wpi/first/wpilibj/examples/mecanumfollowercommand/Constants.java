/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.examples.mecanumfollowercommand;

import edu.wpi.first.wpilibj.geometry.Translation2d;
import edu.wpi.first.wpilibj.kinematics.MecanumDriveKinematics;
import edu.wpi.first.wpilibj.trajectory.TrapezoidProfile;

/**
 * The Constants class provides a convenient place for teams to hold robot-wide numerical or boolean
 * constants.  This class should not be used for any other purpose.  All constants should be
 * declared globally (i.e. public static).  Do not put anything functional in this class.
 *
 * <p>It is advised to statically import this class (or one of its inner classes) wherever the
 * constants are needed, to reduce verbosity.
 */
public final class Constants {
  public static final class DriveConstants {
    public static final int kFrontLeftMotorPort = 0;
    public static final int kRearLeftMotorPort = 1;
    public static final int kFrontRightMotorPort = 2;
    public static final int kRearRightMotorPort = 3;

    public static final int[] kFrontLeftEncoderPorts = new int[]{0, 1};
    public static final int[] kRearLeftEncoderPorts = new int[]{2, 3};
    public static final int[] kFrontRightEncoderPorts = new int[]{4, 5};
    public static final int[] kRearRightEncoderPorts = new int[]{5, 6};

    public static final boolean kFrontLeftEncoderReversed = false;
    public static final boolean kRearLeftEncoderReversed = true;
    public static final boolean kFrontRightEncoderReversed = false;
    public static final boolean kRearRightEncoderReversed = true;

    public static final double kTrackWidth = 0.5;
    // Distance between centers of right and left wheels on robot
    public static final double kTrackLength = 0.7;
    // Distance between centers of front and back wheels on robot

    public static final MecanumDriveKinematics kDriveKinematics =
        new MecanumDriveKinematics(
          new Translation2d(kTrackLength / 2, kTrackWidth / 2),
          new Translation2d(kTrackLength / 2, -kTrackWidth / 2),
          new Translation2d(-kTrackLength / 2, kTrackWidth / 2),
          new Translation2d(-kTrackLength / 2, -kTrackWidth / 2));

    public static final int kEncoderCPR = 1024;
    public static final double kWheelDiameterMeters = .15;
    public static final double kEncoderDistancePerPulse =
        // Assumes the encoders are directly mounted on the wheel shafts
        (kWheelDiameterMeters * Math.PI) / (double) kEncoderCPR;

    public static final boolean kGyroReversed = false;

    // These are example values only - DO NOT USE THESE FOR YOUR OWN ROBOT!
    // These characterization values MUST be determined either experimentally or theoretically
    // for *your* robot's drive.
    // The RobotPy Characterization Toolsuite provides a convenient tool for obtaining these
    // values for your robot.
    public static final double ksVolts = 1;
    public static final double kvVoltSecondsPerMeter = .8;
    public static final double kaVoltSecondsSquaredPerMeter = .15;

    // Example value only - as above, this must be tuned for your drive!
    public static final double kPFrontLeftVel = .5;
    public static final double kPRearLeftVel = .5;
    public static final double kPFrontRightVel = .5;
    public static final double kPRearRightVel = .5;

  }

  public static final class OIConstants {
    public static final int kDriverControllerPort = 1;

  }

  public static final class AutoConstants {
    public static final double kMaxSpeedMetersPerSecond = 3;
    public static final double kMaxAccelerationMetersPerSecondSquared = 3;
    public static final double kMaxAngularSpeedRadiansPerSecond = Math.PI;
    public static final double kMaxAngularSpeedRadiansPerSecondSquared = Math.PI;

    public static final double kPXController = .5;
    public static final double kPYController = .5;
    public static final double kPThetaController = .5;

    //Constraint for the motion profilied robot angle controller
    public static final TrapezoidProfile.Constraints kThetaControllerConstraints = 
        new TrapezoidProfile.Constraints(kMaxAngularSpeedRadiansPerSecond,
        kMaxAngularSpeedRadiansPerSecondSquared);

  }
}