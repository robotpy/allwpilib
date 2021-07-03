
package frc.robot.Drivetrain;

import edu.wpi.first.wpilibj.controller.PIDController;
import edu.wpi.first.wpilibj.controller.HolonomicDriveController;
import edu.wpi.first.wpilibj.controller.ProfiledPIDController;
import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.kinematics.ChassisSpeeds;
import edu.wpi.first.wpilibj.kinematics.SwerveModuleState;
import edu.wpi.first.wpilibj.trajectory.Trajectory;
import edu.wpi.first.wpilibj.trajectory.TrapezoidProfile;
import frc.Constants;

public class DrivetrainControl {

    /* Singleton infrastructure */
    private static DrivetrainControl instance;
    public static DrivetrainControl getInstance() {
        if (instance == null) {
            instance = new DrivetrainControl();
        }
        return instance;
    }

    SwerveModuleControl moduleFL;
    SwerveModuleControl moduleFR;
    SwerveModuleControl moduleBL;
    SwerveModuleControl moduleBR;

    HolonomicDriveController hdc = new HolonomicDriveController(
        new PIDController(8.0, 0, 0), //Fwd/Rev Trajectory Tracking PID Controller
        new PIDController(8.0, 0, 0), //Left/Right Trajectory Tracking PID Controller
        new ProfiledPIDController(8.0, 0, 0, //Rotation Trajectory Tracking PID Controller
          new TrapezoidProfile.Constraints(Constants.MAX_ROTATE_SPEED_RAD_PER_SEC * 0.8, 
                                           Constants.MAX_ROTATE_ACCEL_RAD_PER_SEC_2 * 0.8)));


    ChassisSpeeds desChSpd = new ChassisSpeeds(0, 0, 0);

    Pose2d curDesPose = new Pose2d();

    private DrivetrainControl(){

        hdc.setEnabled(true);

        moduleFL = new SwerveModuleControl("FL", Constants.FL_WHEEL_MOTOR_IDX,Constants.FL_AZMTH_MOTOR_IDX,Constants.FL_WHEEL_ENC_A_IDX,Constants.FL_AZMTH_ENC_A_IDX);
        moduleFR = new SwerveModuleControl("FR", Constants.FR_WHEEL_MOTOR_IDX,Constants.FR_AZMTH_MOTOR_IDX,Constants.FR_WHEEL_ENC_A_IDX,Constants.FR_AZMTH_ENC_A_IDX);
        moduleBL = new SwerveModuleControl("BL", Constants.BL_WHEEL_MOTOR_IDX,Constants.BL_AZMTH_MOTOR_IDX,Constants.BL_WHEEL_ENC_A_IDX,Constants.BL_AZMTH_ENC_A_IDX);
        moduleBR = new SwerveModuleControl("BR", Constants.BR_WHEEL_MOTOR_IDX,Constants.BR_AZMTH_MOTOR_IDX,Constants.BR_WHEEL_ENC_A_IDX,Constants.BR_AZMTH_ENC_A_IDX);          

    }

    public void setInputs(double fwdRevCmd, double strafeCmd, double rotateCmd){
        desChSpd = new ChassisSpeeds(fwdRevCmd, strafeCmd, rotateCmd);
        curDesPose = DrivetrainPoseEstimator.getInstance().getEstPose();
    }

    public void setInputs(Trajectory.State desTrajState, Rotation2d desAngle){
        desChSpd = hdc.calculate(DrivetrainPoseEstimator.getInstance().getEstPose(), desTrajState, desAngle);
        curDesPose = new Pose2d(desTrajState.poseMeters.getTranslation(), desAngle);
    }

    public void stop(){
        setInputs(0,0,0);
    }

    public void update(){

        SwerveModuleState[] desModState;

        if(Math.abs(desChSpd.vxMetersPerSecond) > 0.01 | Math.abs(desChSpd.vyMetersPerSecond) > 0.01 | Math.abs(desChSpd.omegaRadiansPerSecond) > 0.01){
            //In motion
            desModState = Constants.m_kinematics.toSwerveModuleStates(desChSpd);
        } else {
            //Home Position
            desModState = new SwerveModuleState[4];
            desModState[0] = new SwerveModuleState(0, Rotation2d.fromDegrees(-45));
            desModState[1] = new SwerveModuleState(0, Rotation2d.fromDegrees(45));
            desModState[2] = new SwerveModuleState(0, Rotation2d.fromDegrees(45));
            desModState[3] = new SwerveModuleState(0, Rotation2d.fromDegrees(-45));
        }

        moduleFL.setDesiredState(desModState[0]);
        moduleFR.setDesiredState(desModState[1]);
        moduleBL.setDesiredState(desModState[2]);
        moduleBR.setDesiredState(desModState[3]);

        double worstError = getMaxErrorMag();

        var curActualSpeed_ftpersec = DrivetrainPoseEstimator.getInstance().getSpeedFtpSec();

        moduleFL.update(curActualSpeed_ftpersec, worstError);
        moduleFR.update(curActualSpeed_ftpersec, worstError);
        moduleBL.update(curActualSpeed_ftpersec, worstError);
        moduleBR.update(curActualSpeed_ftpersec, worstError);
    }


    public SwerveModuleState [] getModuleActualStates(){
        SwerveModuleState retArr[] =  { moduleFL.getActualState(),
                                        moduleFR.getActualState(),
                                        moduleBL.getActualState(),
                                        moduleFR.getActualState()};
        return retArr;
    }

    public SwerveModuleState [] getModuleDesiredStates(){
        SwerveModuleState retArr[] =  { moduleFL.getDesiredState(),
                                        moduleFR.getDesiredState(),
                                        moduleBL.getDesiredState(),
                                        moduleFR.getDesiredState()};
        return retArr;
    }

    public double getMaxErrorMag(){
        double maxErr = 0;
        maxErr = Math.max(maxErr, moduleFL.azmthCtrl.getErrMag_deg());
        maxErr = Math.max(maxErr, moduleFR.azmthCtrl.getErrMag_deg());
        maxErr = Math.max(maxErr, moduleBL.azmthCtrl.getErrMag_deg());
        maxErr = Math.max(maxErr, moduleFR.azmthCtrl.getErrMag_deg());
        return maxErr;
    }

    
    public Pose2d getCurDesiredPose(){
        return curDesPose;
    }

    public void resetWheelEncoders() {
        moduleFL.wheelEnc.reset();
        moduleFR.wheelEnc.reset();
        moduleBL.wheelEnc.reset();
        moduleBR.wheelEnc.reset();
    }


}