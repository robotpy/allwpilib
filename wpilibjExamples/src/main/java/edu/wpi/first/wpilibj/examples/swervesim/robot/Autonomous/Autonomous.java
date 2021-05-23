package frc.robot.Autonomous;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import frc.lib.AutoSequencer.AutoSequencer;
import frc.robot.Autonomous.Modes.AutoDelayMode;
import frc.robot.Autonomous.Modes.AutoMode;
import frc.robot.Autonomous.Modes.AutoModeDoNothing;
import frc.robot.Autonomous.Modes.AutoModeDriveFig8;
import frc.robot.Autonomous.Modes.AutoModeDriveForward3Sec;

public class Autonomous {
    
    // Driver-selectable autonomous mode lists
    AutoModeList delayModes;
    AutoModeList mainModes;

    AutoSequencer seq;

    AutoMode curDelayMode = null;
    AutoMode prevDelayMode = null;
    AutoMode curMainMode = null;
    AutoMode prevMainMode = null;

    public Autonomous(){

        seq = new AutoSequencer("Auto Event Sequencer");

        delayModes = new AutoModeList();
        mainModes = new AutoModeList();

        //Available Delay Modes
        delayModes.add(new AutoDelayMode(0.0)); //First is default
        delayModes.add(new AutoDelayMode(3.0));
        delayModes.add(new AutoDelayMode(6.0));
        delayModes.add(new AutoDelayMode(9.0));
        delayModes.add(new AutoDelayMode(12.0));

        //Available Main Modes
        mainModes.add(new AutoModeDriveFig8()); //First is default
        mainModes.add(new AutoModeDriveForward3Sec()); 
        mainModes.add(new AutoModeDoNothing());

        initDashboard();

        reset();
    }

    private void initDashboard(){

    }

    private void readFromDashboard(){

    }

    public void loadSequencer(){
        System.out.println("Loading new autonomous routines...");
        System.out.println("DELAY: " + curDelayMode.humanReadableName);
        System.out.println("MAIN: " + curMainMode.humanReadableName);

        seq.clearAllEvents();

        curDelayMode.addStepsToSequencer(seq);
        curMainMode.addStepsToSequencer(seq);

        System.out.println("Finished loading new auto routines!");
    }

    public void modeUpdate(){

        readFromDashboard();

        if(curDelayMode != prevDelayMode || curMainMode != prevMainMode){
            loadSequencer();
        }

        prevDelayMode = curDelayMode;
        prevMainMode = curMainMode;
    }

    public void sequencerUpdate(){
        seq.update();
    }

    public void reset(){
        curDelayMode = delayModes.getDefault();
        curMainMode = mainModes.getDefault();
        loadSequencer();
    }

    public void stop(){
        seq.stop();
        sequencerUpdate();
    }

    public void start(){
        seq.start();
        sequencerUpdate();
    }

    public boolean isActive(){
        return seq.isRunning();
    }

	public Pose2d getStartPose() {
		return curMainMode.getInitialPose();
	}
}
