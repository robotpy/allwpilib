package frc.sim;

import edu.wpi.first.wpilibj.simulation.FlywheelSim;
import edu.wpi.first.wpilibj.system.plant.DCMotor;
import frc.Constants;

class SimpleMotorWithMassModel {

    
    double speedAct_RPM;
    
    double current_A;
    
    double curDisplacement_Rev;

    FlywheelSim fwSim;

    public SimpleMotorWithMassModel(DCMotor motor, double gearing, double moi){
        fwSim = new FlywheelSim(motor, gearing, moi);
    }

    /**
     * Set the motor back to zero speed and zero current draw.
     */
    public void modelReset(){
        
    }

    /**
     * Step through one loop of simulation for the motor
     * @param supplyVoltage_in Present battery supply voltage to the controller. Nominally 12.5 or so, but reduce to simulatle a dying battery
     * @param motorCommand_in Speed controller command - 1.0 = full fwd, 0.0 = stop, -1.0 = full reverse
     */
    public void update(double supplyVoltage_in, double motorCommand_in){

        fwSim.setInputVoltage(supplyVoltage_in * motorCommand_in);

        fwSim.update(Constants.SIM_SAMPLE_RATE_SEC);

        speedAct_RPM = fwSim.getAngularVelocityRPM();
        current_A = fwSim.getCurrentDrawAmps();

        curDisplacement_Rev += speedAct_RPM / 60 * Constants.SIM_SAMPLE_RATE_SEC;

    }

    /**
     * 
     * @return The present speed of the rotating mass
     */
    double getSpeed_RPM(){
        return speedAct_RPM;
    }

    /**
     * 
     * @return The present current draw of the mechanism
     */
    double getCurrent_A(){
        return current_A;
    }

    /**
     * 
     * @return The present displacement in Revolutions
     */
    double getPosition_Rev(){
        return curDisplacement_Rev;
    }


}