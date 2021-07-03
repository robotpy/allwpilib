package frc.sim;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import frc.Constants;
import frc.robot.SimableWrappers.SimulatableADXRS450;

public class SimGyroSensorModel{

    SimulatableADXRS450 gyroSim;
    double gyroPosReading_deg;

    // Limit what the gyro itself can read
    // Ex for ADSRX453 - https://www.analog.com/media/en/technical-documentation/data-sheets/ADXRS453.pdf
    final double GYRO_RATE_SCALING_DEGPERSEC_PER_BIT = 1.0/80.0;
    final double GYRO_MAX_MEASURABLE_RATE_DEGPERSEC = 400.0;

    public SimGyroSensorModel(){
        gyroSim = new SimulatableADXRS450();

    }

    public void resetToPose(Pose2d resetPose){
        gyroSim.simSetAngle(resetPose.getRotation().getDegrees() * -1.0);
    }

    public void update(Pose2d curRobotPose, Pose2d prevRobotPose){

        double curGyroAngle  = curRobotPose.getRotation().getDegrees();
        double prevGyroAngle = prevRobotPose.getRotation().getDegrees();
        double gyroRate = -1.0 * (curGyroAngle - prevGyroAngle)/Constants.SIM_SAMPLE_RATE_SEC; //Gyro reads backward from sim reference frames.
        
        gyroRate = Math.min(gyroRate,  GYRO_MAX_MEASURABLE_RATE_DEGPERSEC);
        gyroRate = Math.max(gyroRate, -GYRO_MAX_MEASURABLE_RATE_DEGPERSEC);

        //Round gyro rate to gyro internal scaling
        long gyroBits = Math.round(gyroRate / GYRO_RATE_SCALING_DEGPERSEC_PER_BIT);

        gyroRate = gyroBits * GYRO_RATE_SCALING_DEGPERSEC_PER_BIT;
        
        // Pass our model of what the sensor would be measuring back into the simGyro object
        // for the embedded code to interact with.
        gyroSim.simUpdate(gyroRate, Constants.SIM_SAMPLE_RATE_SEC);
    }
}