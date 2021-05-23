package frc.robot.Autonomous.Modes.Events;

import edu.wpi.first.wpilibj.Timer;
import frc.lib.AutoSequencer.AutoEvent;
import frc.robot.Drivetrain.DrivetrainControl;

public class AutoEventDriveFwdTime extends AutoEvent {
    double duration_s;
    double endTime;
    
    double speed_mps;
    boolean done = false;
	
	public AutoEventDriveFwdTime(double duration_s_in, double speed_mps_in) {
        duration_s = duration_s_in;
        speed_mps = speed_mps_in;
	}

	@Override
	public void userStart() {
        endTime = Timer.getFPGATimestamp() + duration_s;
        done = false;
	}

	@Override
	public void userUpdate() {
        if(Timer.getFPGATimestamp() < endTime){
            DrivetrainControl.getInstance().setInputs(speed_mps, 0, 0);
            done = false;
        } else {
            DrivetrainControl.getInstance().stop();
            done = true;
        }
		return;
	}

	@Override
	public void userForceStop() {
        DrivetrainControl.getInstance().stop();
        done = true;
		return;
	}

	@Override
	public boolean isTriggered() {
		return true;
	}

	@Override
	public boolean isDone() {
		return done;
	}
}
