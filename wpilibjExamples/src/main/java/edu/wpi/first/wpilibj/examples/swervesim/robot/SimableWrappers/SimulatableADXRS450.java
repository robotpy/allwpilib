package frc.robot.SimableWrappers;

import edu.wpi.first.wpilibj.ADXRS450_Gyro;
import frc.SimDataInf;
import frc.robot.Robot;

/**
 * Wrapper around the gyro used in robot code to redirect to the real gyro
 * when on a real robot, or use the simulation data interface if we are
 * running in simulation.
 * 
 */
public class SimulatableADXRS450 {

    /**
     * Real gyroscope on the real robot
     */
    ADXRS450_Gyro realGyro;

    boolean isReal;

    public SimulatableADXRS450(){
        isReal = Robot.isReal();

        if(isReal){
            realGyro = new ADXRS450_Gyro();
        } else {
            realGyro = null;
        }
    }

    public void reset(){
        if(isReal){
            realGyro.reset();
        } else {
            SimDataInf.gyro_rates[0] = 0;
            SimDataInf.gyro_angles[0] = 0;
        }

    }

    public void calibrate(){
        if(isReal){
            realGyro.calibrate();
        } else {
            //nada;
        }

    }

    public double getRate(){
        if(isReal){
            return realGyro.getRate();
        } else {
            return SimDataInf.gyro_rates[0];
        }
    }

    public double getAngle(){
        if(isReal){
            return realGyro.getAngle();
        } else {
            return SimDataInf.gyro_angles[0];
        }
    }

    public boolean isConnected(){
        if(isReal){
            return realGyro.isConnected();
        } else {
            return true;
        }
    }

    public void simUpdate(double newRate, double sampleTime){
        SimDataInf.gyro_rates[0] = newRate;
        SimDataInf.gyro_angles[0] += newRate * sampleTime;
    }   

    public void simSetAngle(double newAngle){
        SimDataInf.gyro_rates[0] = 0;
        SimDataInf.gyro_angles[0] = newAngle;
    }   





    
}
