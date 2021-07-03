package frc.robot.Drivetrain;

import edu.wpi.first.wpilibj.geometry.Rotation2d;
import frc.robot.SimableWrappers.SimulatableADXRS450;

/**
 * Our favorite ADXRS450 and 453 gyro doesn't yet fully support
 * all required features for angles with Rotation2d's and restability, 
 * so we're adding this wrapper to provide those features;
 */
public class WrapperedADXRS450 {

    SimulatableADXRS450 gyro;

    Rotation2d readingOffset = Rotation2d.fromDegrees(0);

    
    double rawGyroAngle = 0;

    public WrapperedADXRS450(){
        gyro = new SimulatableADXRS450(); 
    } 

    public void resetToAngle(Rotation2d angle){
        gyro.reset();
        readingOffset = angle;
    }

    public double getRate(){
        return gyro.getRate();
    }

    public Rotation2d getAngle(){
        rawGyroAngle = gyro.getAngle();
        return Rotation2d.fromDegrees(rawGyroAngle).minus(readingOffset);
    }

    public void calibrate(){
        gyro.calibrate();
    }

    public boolean isConnected(){
        return gyro.isConnected();
    }


}