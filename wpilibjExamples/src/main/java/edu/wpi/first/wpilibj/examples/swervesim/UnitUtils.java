package frc;

import edu.wpi.first.wpilibj.util.Units;

public class UnitUtils{

    public static double DtRPMtoMPerSec(double rot_spd_in){ return rot_spd_in * (1.0/60.0) * (2.0 * Math.PI * Units.inchesToMeters(Constants.WHEEL_RADIUS_IN)); }
    public static double DtMPerSectoRPM(double lin_spd_in){ return lin_spd_in / (1.0/60.0) / (2.0 * Math.PI * Units.inchesToMeters(Constants.WHEEL_RADIUS_IN)); }
    public static double RPMtoDegPerSec(double rpmIn){ return rpmIn * 60.0 / 360.0; }
    public static double lbsToKg(double lbs_in){return 0.4535924 * lbs_in;}

    public static double wrapAngleDeg(double angle){
        angle %=360;
        angle= angle>180 ? angle-360 : angle;
        angle= angle<-180 ? angle+360 : angle;
        return angle;
    }

    public static double limitMotorCmd(double motorCmdIn){
        return Math.max(Math.min(motorCmdIn,1.0),-1.0);
    }

    public static void main(String[] args){
        for(double in = -9999; in < 9999; in++){
            double out = wrapAngleDeg(in);
            if(Math.abs(out) > 180){
                System.out.println("IN:" + Double.toString(in) + " OUT:" + Double.toString(out));
            }
        }
        System.out.println("Done!");
    }
}