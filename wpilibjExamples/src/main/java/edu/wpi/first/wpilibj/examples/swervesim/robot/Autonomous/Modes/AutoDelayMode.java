package frc.robot.Autonomous.Modes;

import frc.lib.AutoSequencer.AutoSequencer;
import frc.robot.Autonomous.Modes.Events.AutoEventWait;

public class AutoDelayMode extends AutoMode{

    private double delayDur_sec = 0;

    public AutoDelayMode(double duration_sec){
        delayDur_sec = duration_sec;
        humanReadableName = Double.toString(delayDur_sec) + "sec";
    }

    @Override
    public void addStepsToSequencer(AutoSequencer seq){
        seq.addEvent(new AutoEventWait(delayDur_sec));
    }
    
}
