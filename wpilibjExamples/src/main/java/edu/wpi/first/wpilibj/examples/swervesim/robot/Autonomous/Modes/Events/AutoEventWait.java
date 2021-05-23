package frc.robot.Autonomous.Modes.Events;

import edu.wpi.first.wpilibj.Timer;
import frc.lib.AutoSequencer.AutoEvent;

public class AutoEventWait extends AutoEvent {
	
	double duration_s;
	double endTime;
	
	public AutoEventWait(double duration_s_in) {
		duration_s = duration_s_in;
	}

	@Override
	public void userStart() {
		endTime = Timer.getFPGATimestamp() + duration_s;
	}

	@Override
	public void userUpdate() {
		return;
	}

	@Override
	public void userForceStop() {
		return;
	}

	@Override
	public boolean isTriggered() {
		return true;
	}

	@Override
	public boolean isDone() {
		return Timer.getFPGATimestamp() >= endTime;
	}
}