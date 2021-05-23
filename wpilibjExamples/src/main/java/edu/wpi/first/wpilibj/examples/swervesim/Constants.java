package frc;

import java.util.Arrays;
import java.util.List;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.geometry.Transform2d;
import edu.wpi.first.wpilibj.geometry.Translation2d;
import edu.wpi.first.wpilibj.kinematics.SwerveDriveKinematics;
import edu.wpi.first.wpilibj.util.Units;

public class Constants {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////  Configured Constants
    //// - These are tied to the specifics on how your mechanical team,
    ////   electrical team, and the game design committee did their work.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ROBOT PHYSICAL CONSTANTS
    // Robot physical dimensions and mass quantities.
    static public final double WHEEL_BASE_HALF_WIDTH_M = Units.feetToMeters(1.0);
    static public final double WHEEL_RADIUS_IN = 6.0/2.0; //six inch diameter wheels
    static public final double ROBOT_MASS_kg = UnitUtils.lbsToKg(140);
    static public final double ROBOT_MOI_KGM2 = 1.0/12.0 * ROBOT_MASS_kg * Math.pow((WHEEL_BASE_HALF_WIDTH_M*2.2),2) * 2; //Model moment of intertia as a square slab slightly bigger than wheelbase with axis through center
    // Location of vision camera relative to robot center - currently front middle.
    static public final Transform2d robotToCameraTrans = new Transform2d(new Translation2d(WHEEL_BASE_HALF_WIDTH_M, 0), new Rotation2d(0.0));
    // Drivetrain Performance Mechanical limits
    static public final double MAX_FWD_REV_SPEED_MPS = Units.feetToMeters(12.0);
    static public final double MAX_STRAFE_SPEED_MPS = Units.feetToMeters(8.0);
    static public final double MAX_ROTATE_SPEED_RAD_PER_SEC = Units.degreesToRadians(360.0);
    static public final double MAX_TRANSLATE_ACCEL_MPS2 = MAX_FWD_REV_SPEED_MPS/0.25; //0-full time of 0.25 second
    static public final double MAX_ROTATE_ACCEL_RAD_PER_SEC_2 = MAX_ROTATE_SPEED_RAD_PER_SEC/0.25; //0-full time of 0.25 second


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ROBOT ELECTRICAL CONSTANTS - update these to match how you wired your robot.
    // PWM Bank
    static public final int FL_WHEEL_MOTOR_IDX = 0;
    static public final int FL_AZMTH_MOTOR_IDX = 1;
    static public final int FR_WHEEL_MOTOR_IDX = 2;
    static public final int FR_AZMTH_MOTOR_IDX = 3;
    static public final int BL_WHEEL_MOTOR_IDX = 4;
    static public final int BL_AZMTH_MOTOR_IDX = 5;
    static public final int BR_WHEEL_MOTOR_IDX = 6;
    static public final int BR_AZMTH_MOTOR_IDX = 7;
    //static public final int UNUSED = 8;
    //static public final int UNUSED = 9;
    // DIO Bank
    static public final int FL_WHEEL_ENC_A_IDX = 0;
    static public final int FL_WHEEL_ENC_B_IDX = 1;
    static public final int FL_AZMTH_ENC_A_IDX = 2;
    static public final int FL_AZMTH_ENC_B_IDX = 3;
    static public final int FR_WHEEL_ENC_A_IDX = 4;
    static public final int FR_WHEEL_ENC_B_IDX = 5;
    static public final int FR_AZMTH_ENC_A_IDX = 6;
    static public final int FR_AZMTH_ENC_B_IDX = 7;
    static public final int BL_WHEEL_ENC_A_IDX = 8;
    static public final int BL_WHEEL_ENC_B_IDX = 9;
    static public final int BL_AZMTH_ENC_A_IDX = 10;
    static public final int BL_AZMTH_ENC_B_IDX = 11;
    static public final int BR_WHEEL_ENC_A_IDX = 12;
    static public final int BR_WHEEL_ENC_B_IDX = 13;
    static public final int BR_AZMTH_ENC_A_IDX = 14;
    static public final int BR_AZMTH_ENC_B_IDX = 15;
    //static public final int UNUSED = 16;
    //static public final int UNUSED = 17;
    //static public final int UNUSED = 18;
    //static public final int UNUSED = 19;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // SENSOR CONSTANTS
    // Sensor-related constants - pulled from datasheets for the sensors and gearboxes
    static public final int ENC_PULSE_PER_REV = 1024;
    static public final int WHEEL_ENC_COUNTS_PER_WHEEL_REV = ENC_PULSE_PER_REV;  //Assume 1-1 gearing for now
    static public final int AZMTH_ENC_COUNTS_PER_MODULE_REV = ENC_PULSE_PER_REV; //Assume 1-1 gearing for now
    static public final double WHEEL_ENC_WHEEL_REVS_PER_COUNT  = 1.0/((double)(Constants.WHEEL_ENC_COUNTS_PER_WHEEL_REV));
    static public final double AZMTH_ENC_MODULE_REVS_PER_COUNT = 1.0/((double)(Constants.AZMTH_ENC_COUNTS_PER_MODULE_REV));
    // Vision Camera
    static public final String PHOTON_CAM_NAME = "MainCamera";


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // FIELD PHYSICAL CONSTANTS
    // Field wall locations which limit drivetrain motion
    static public final double FIELD_WIDTH_M = Units.feetToMeters(27.0);
    static public final double FIELD_LENGTH_M = Units.feetToMeters(54.0);
    static public final Translation2d MAX_ROBOT_TRANSLATION = new Translation2d(FIELD_LENGTH_M, FIELD_WIDTH_M);
    static public final Translation2d MIN_ROBOT_TRANSLATION = new Translation2d(0.0,0.0);
    // Assumed starting location of the robot. Auto routines will pick their own location and update this.
    static public final Pose2d DFLT_START_POSE = new Pose2d(Units.feetToMeters(24.0), Units.feetToMeters(10.0), Rotation2d.fromDegrees(180));
    // Expected vision target locations on the field
    static public final Transform2d fieldToFarVisionTargetTrans   = new Transform2d( new Translation2d(FIELD_LENGTH_M, Units.feetToMeters(9.8541)), Rotation2d.fromDegrees(0));
    static public final Transform2d fieldToCloseVisionTargetTrans = new Transform2d( new Translation2d(Units.feetToMeters(0), Units.feetToMeters(17.14)), Rotation2d.fromDegrees(180));



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////  Derived Constants
    //// - You can reference how these are calculated, but shouldn't
    ////   have to change them
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // HELPER ORGANIZATION CONSTANTS
    static public final int FL = 0; // Front Left Module Index
    static public final int FR = 1; // Front Right Module Index
    static public final int BL = 2; // Back Left Module Index
    static public final int BR = 3; // Back Right Module Index
    static public final int NUM_MODULES = 4;

    // Internal objects used to track where the modules are at relative to
    // the center of the robot, and all the implications that spacing has.
    static public final List<Translation2d> robotToModuleTL = Arrays.asList(
        new Translation2d( Constants.WHEEL_BASE_HALF_WIDTH_M,  Constants.WHEEL_BASE_HALF_WIDTH_M),
        new Translation2d( Constants.WHEEL_BASE_HALF_WIDTH_M, -Constants.WHEEL_BASE_HALF_WIDTH_M),
        new Translation2d(-Constants.WHEEL_BASE_HALF_WIDTH_M,  Constants.WHEEL_BASE_HALF_WIDTH_M),
        new Translation2d(-Constants.WHEEL_BASE_HALF_WIDTH_M, -Constants.WHEEL_BASE_HALF_WIDTH_M)
    );

    static public final List<Transform2d> robotToModuleTF = Arrays.asList(
        new Transform2d(robotToModuleTL.get(FL), new Rotation2d(0.0)),
        new Transform2d(robotToModuleTL.get(FR), new Rotation2d(0.0)),
        new Transform2d(robotToModuleTL.get(BL), new Rotation2d(0.0)),
        new Transform2d(robotToModuleTL.get(BR), new Rotation2d(0.0))
    );

    static public final SwerveDriveKinematics m_kinematics = new SwerveDriveKinematics(
        robotToModuleTL.get(FL), 
        robotToModuleTL.get(FR), 
        robotToModuleTL.get(BL), 
        robotToModuleTL.get(BR)
    );

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MISC CONSTANTS
    // Nominal Periodic code execution rates
    static public final double SIM_SAMPLE_RATE_SEC = 0.001;
    static public final double CTRLS_SAMPLE_RATE_SEC = 0.02;



}