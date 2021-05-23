package frc.sim;

import edu.wpi.first.wpilibj.system.plant.DCMotor;
import frc.Constants;

public class DCMotorSim {

    DCMotor motorParams;

    
    double curCurrent_A = 0;

    double prevCurrent_A = 0;

    double prevSpeed_radpsec = 0;

    
    double curTorque_Nm = 0;

    final double effective_moi = 0.5 * 0.9 * 0.0254 * 0.0254; //two inch diameter, one pound cylinder, rotated about center axis

    public DCMotorSim(DCMotor motorParams_in){
        motorParams = motorParams_in;
    }

    public void update(double speed_radpsec, double voltage_v){
        prevCurrent_A = curCurrent_A;
        curCurrent_A = motorParams.getCurrent(speed_radpsec, voltage_v);
        double accel_radpsec2 = (speed_radpsec - prevSpeed_radpsec) / (Constants.SIM_SAMPLE_RATE_SEC);
        
        //Torque output = torque due to current - torque due to acceleration
        curTorque_Nm = motorParams.KtNMPerAmp * curCurrent_A + effective_moi*accel_radpsec2;
        
        prevSpeed_radpsec = speed_radpsec;
    }

    public double getTorque_Nm(){
        return curTorque_Nm;
    }

    public double getCurrent_A(){
        return curCurrent_A;
    }
}