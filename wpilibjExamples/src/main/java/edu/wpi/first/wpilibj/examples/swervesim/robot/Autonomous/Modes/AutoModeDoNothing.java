package frc.robot.Autonomous.Modes;

import frc.lib.AutoSequencer.AutoSequencer;

public class AutoModeDoNothing extends AutoMode {

    public AutoModeDoNothing(){
        humanReadableName = "Do Nothing";
    }

    @Override
    public void addStepsToSequencer(AutoSequencer seq){
        //Do nothing!
    }

    
}
