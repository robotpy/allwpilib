package frc.robot.Autonomous.Modes;

import frc.lib.AutoSequencer.AutoSequencer;
import frc.robot.Autonomous.Modes.Events.AutoEventDriveFwdTime;

public class AutoModeDriveForward3Sec extends AutoMode {

    public AutoModeDriveForward3Sec(){
        humanReadableName = "Drive Forward for 3 Sec";
    }

    @Override
    public void addStepsToSequencer(AutoSequencer seq){
        seq.addEvent(new AutoEventDriveFwdTime(3.0, 0.5));
    }

    
}
