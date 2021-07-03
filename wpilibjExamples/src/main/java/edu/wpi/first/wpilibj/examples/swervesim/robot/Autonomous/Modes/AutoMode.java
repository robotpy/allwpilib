package frc.robot.Autonomous.Modes;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import frc.Constants;
import frc.lib.AutoSequencer.AutoSequencer;

public abstract class AutoMode {

    public String humanReadableName = "";

    public abstract void addStepsToSequencer(AutoSequencer seq);
    
    public Pose2d getInitialPose(){
        return Constants.DFLT_START_POSE;
    }
}
