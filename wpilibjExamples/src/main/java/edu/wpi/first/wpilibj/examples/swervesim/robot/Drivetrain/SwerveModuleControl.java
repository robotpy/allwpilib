
package frc.robot.Drivetrain;

import edu.wpi.first.wpilibj.Encoder;
import edu.wpi.first.wpilibj.Spark;
import edu.wpi.first.wpilibj.controller.PIDController;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.kinematics.SwerveModuleState;
import frc.Constants;
import frc.UnitUtils;
import frc.lib.Util.MapLookup2D;

class SwerveModuleControl {

    Spark wheelMotorCtrl;
    Spark azmthMotorCtrl;
    Encoder wheelEnc;
    Encoder azmthEnc;

    SwerveModuleState desState = new SwerveModuleState();
    SwerveModuleState actState = new SwerveModuleState();

    double wheelMotorSpeedDes_RPM = 0;
    double wheelMotorSpeedAct_RPM = 0;

    double azmthPosAct_deg = 0;

    
    double wheelMotorCmd;

    AzimuthAngleController azmthCtrl;

    MapLookup2D wheelCmdLimitTbl;


    final double WHEEL_MAX_SPEED_RPM = 620; //determined empirically

    PIDController wheelPIDCtrl = new PIDController(0.011, 0, 0.000);


    public SwerveModuleControl(String posId, int wheelMotorIdx, int azmthMotorIdx, int wheelEncoderIdx, int azmthEncoderIdx){

        wheelMotorCtrl = new Spark(wheelMotorIdx);
        azmthMotorCtrl = new Spark(azmthMotorIdx);
        wheelEnc = new Encoder(wheelEncoderIdx, wheelEncoderIdx + 1); //Always assume channel B is one after channel A.
        azmthEnc = new Encoder(azmthEncoderIdx, azmthEncoderIdx + 1);

        wheelEnc.setDistancePerPulse(Constants.WHEEL_ENC_WHEEL_REVS_PER_COUNT);
        azmthEnc.setDistancePerPulse(Constants.AZMTH_ENC_MODULE_REVS_PER_COUNT);

        wheelCmdLimitTbl = new MapLookup2D();
        wheelCmdLimitTbl.insertNewPoint(0.0, 1.0);
        wheelCmdLimitTbl.insertNewPoint(5.0, 1.0);
        wheelCmdLimitTbl.insertNewPoint(7.0, 0.8);
        wheelCmdLimitTbl.insertNewPoint(15.0, 0.5);
        wheelCmdLimitTbl.insertNewPoint(30.0, 0.1);
        wheelCmdLimitTbl.insertNewPoint(45.0, 0.0);
        wheelCmdLimitTbl.insertNewPoint(90.0, 0.0);

        azmthCtrl = new AzimuthAngleController();

    }

    public void update(double curSpeedFtPerSec, double maxAzmthErr_deg){

        azmthPosAct_deg = azmthEnc.getDistance() * 360.0;

        azmthCtrl.setInputs(desState.angle.getDegrees(), azmthPosAct_deg, curSpeedFtPerSec);
        azmthCtrl.update();

        //Calcaulte desired speed from input state, azimuth controller reversal command, and worst-case azimuth module error.
        wheelMotorSpeedDes_RPM = UnitUtils.DtMPerSectoRPM(desState.speedMetersPerSecond)*(azmthCtrl.getInvertWheelCmd()?-1.0:1.0);
        wheelMotorSpeedDes_RPM *= wheelCmdLimitTbl.lookupVal(maxAzmthErr_deg);

        wheelMotorSpeedAct_RPM = wheelEnc.getRate() * 60;

        //Closed-loop control of wheel velocity
        wheelPIDCtrl.setSetpoint(wheelMotorSpeedDes_RPM);
        double wheelFFCmd = wheelMotorSpeedDes_RPM/WHEEL_MAX_SPEED_RPM;
        double wheelFBCmd = wheelPIDCtrl.calculate(wheelMotorSpeedAct_RPM);
        wheelMotorCmd = UnitUtils.limitMotorCmd(wheelFFCmd+wheelFBCmd);

        wheelMotorCtrl.set(wheelMotorCmd); 
        azmthMotorCtrl.set(azmthCtrl.getMotorCmd()); 

        actState.angle = Rotation2d.fromDegrees(azmthPosAct_deg);
        actState.speedMetersPerSecond = UnitUtils.DtRPMtoMPerSec(wheelMotorSpeedAct_RPM);

        updateTelemetry();
    }

    /**
     * Broadcast signals specific to the visualiation
     */
    public void updateTelemetry(){

    }

    public void setDesiredState(SwerveModuleState des){
        desState = des;
    }

    public SwerveModuleState getActualState(){
        return actState;
    }

    public SwerveModuleState getDesiredState(){
        return desState;
    }

}
