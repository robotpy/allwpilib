package frc.robot.HumanInterface;

import edu.wpi.first.wpilibj.SlewRateLimiter;
import edu.wpi.first.wpilibj.XboxController;
import frc.Constants;

public class DriverInterface{

    final double JOY_DEADBAND = 0.1;

    XboxController ctrl;

    
    double fwdRevSpdCmd = 0;
    
    double strafeSpdCmd = 0;
    
    double rotateCmd = 0;

    SlewRateLimiter fwdRevSlew;
    SlewRateLimiter strafeSlew;
    SlewRateLimiter rotateSlew;

    public DriverInterface(){
        ctrl = new XboxController(0);

        fwdRevSlew = new SlewRateLimiter(Constants.MAX_TRANSLATE_ACCEL_MPS2);
        strafeSlew = new SlewRateLimiter(Constants.MAX_TRANSLATE_ACCEL_MPS2);
        rotateSlew = new SlewRateLimiter(Constants.MAX_ROTATE_ACCEL_RAD_PER_SEC_2);

    }

    public void update(){
        fwdRevSpdCmd = -1.0*applyDeadband(ctrl.getRawAxis(1))*Constants.MAX_FWD_REV_SPEED_MPS;
        strafeSpdCmd = -1.0*applyDeadband(ctrl.getRawAxis(0))*Constants.MAX_STRAFE_SPEED_MPS;
        rotateCmd    = -1.0*applyDeadband(ctrl.getRawAxis(2))*Constants.MAX_ROTATE_SPEED_RAD_PER_SEC;

        fwdRevSpdCmd = fwdRevSlew.calculate(fwdRevSpdCmd);
        strafeSpdCmd = strafeSlew.calculate(strafeSpdCmd);
        rotateCmd    = rotateSlew.calculate(rotateCmd);
    }

    public double getFwdRevSpeedCmd_mps(){
        return fwdRevSpdCmd;
    }

    public double getStrafeSpeedCmd_mps(){
        return strafeSpdCmd;
    }

    public double getRotateCmd_radPerSec(){
        return rotateCmd;
    }

    private double applyDeadband(double input){
        if(Math.abs(input) < JOY_DEADBAND){
            return 0.0;
        } else {
            return Math.signum(input) * ( (Math.abs(input) - JOY_DEADBAND) * 1.0 / (1.0 - JOY_DEADBAND) );
        }
        
    }
}
