package frc.sim;

import edu.wpi.first.wpilibj.system.plant.DCMotor;
import edu.wpi.first.wpilibj.util.Units;
import frc.Constants;

public class MotorGearboxWheelSim {
    
    DCMotorSim motor;
    double gearRatio;
    double wheelRadius_m;
    double curGroundForce_N;
    double wheelRotations_rad;
    double gearboxFricCoef_NmPerRadPerSec;
    double prevWheelRotationalSpeed_radPerSec;

    
    double wheelSpeed_RPM;

    
    double motorSpeed_RPM;

    /**
     * 1D simulation of  a motor, gearbox, and wheel.
     * Provides an output torque, given an input speed and voltage.
     * @param motor_in Motor set used to provide power input.
     * @param gearRatio_in Gearbox Ratio. Gearboxes which slow down motors (ie all of them) should have this greater than 1.0.
     * @param wheelDiameter_m_in Diameter of the wheel in meters
     * @param gearboxFricCoef_NmPerRadPerSec_in Kinetic Friction Losses in the gearbox (expressed in units of Nm of "fighting" force per radian per second of motor speed). Set to 0 if you're awesome with white lithium grease, make it positive if your freshman maybe forget the grease sometimes.
     */
    public MotorGearboxWheelSim(DCMotor motor_in, double gearRatio_in, double wheelDiameter_m_in, double gearboxFricCoef_NmPerRadPerSec_in){
        motor = new DCMotorSim(motor_in);
        gearRatio = gearRatio_in;
        wheelRadius_m = wheelDiameter_m_in/2.0;
        gearboxFricCoef_NmPerRadPerSec = gearboxFricCoef_NmPerRadPerSec_in;
    }

    public void update(double groundVelocity_mps, double supplyVoltage_in, double motorCommand_in){

        double wheelRotationalSpeed_radPerSec = groundVelocity_mps / wheelRadius_m;
        double motorRotationalSpeed_radPerSec = wheelRotationalSpeed_radPerSec * gearRatio;

        motor.update(motorRotationalSpeed_radPerSec, supplyVoltage_in*motorCommand_in);

        double gearboxFrictionalTorque_Nm = motorRotationalSpeed_radPerSec * gearboxFricCoef_NmPerRadPerSec;
        double curWheelTorque_Nm = motor.getTorque_Nm() * gearRatio  - gearboxFrictionalTorque_Nm; //div by 1/torque ratio 
        
        curGroundForce_N = curWheelTorque_Nm / wheelRadius_m / 2;

        wheelRotations_rad += (wheelRotationalSpeed_radPerSec + prevWheelRotationalSpeed_radPerSec)/2 * Constants.SIM_SAMPLE_RATE_SEC; //Trapezoidal integration

        prevWheelRotationalSpeed_radPerSec = wheelRotationalSpeed_radPerSec;

        wheelSpeed_RPM = Units.radiansPerSecondToRotationsPerMinute(wheelRotationalSpeed_radPerSec);
        motorSpeed_RPM = Units.radiansPerSecondToRotationsPerMinute(motorRotationalSpeed_radPerSec);
    }

    public double getPosition_Rev(){
        return wheelRotations_rad / 2 / Math.PI;
    }

    public double getGroundForce_N(){
        return curGroundForce_N;
    }

    public double getCurrent_A(){
        return motor.getCurrent_A();
    }
}