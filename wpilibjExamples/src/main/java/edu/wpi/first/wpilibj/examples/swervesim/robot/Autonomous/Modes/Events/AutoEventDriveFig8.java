package frc.robot.Autonomous.Modes.Events;

import java.util.ArrayList;

import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import edu.wpi.first.wpilibj.geometry.Translation2d;
import edu.wpi.first.wpilibj.trajectory.Trajectory;
import edu.wpi.first.wpilibj.trajectory.TrajectoryConfig;
import edu.wpi.first.wpilibj.trajectory.TrajectoryGenerator;
import edu.wpi.first.wpilibj.trajectory.constraint.SwerveDriveKinematicsConstraint;
import frc.Constants;
import frc.lib.AutoSequencer.AutoEvent;
import frc.robot.Drivetrain.DrivetrainControl;

public class AutoEventDriveFig8 extends AutoEvent {

	double startTime = 0;
	boolean done = false;

	TrajectoryConfig config;
	Trajectory traj;

	Rotation2d startOrientation;
	Rotation2d endOrientation;
	
	public AutoEventDriveFig8() {

		//////////////////////////////////////////////////////
		// Trajectory Configuration
		Pose2d startPose = new Pose2d(new Translation2d(3.0, 3.0), Rotation2d.fromDegrees(0));
		Pose2d endPose = new Pose2d(new Translation2d(3.0, 3.0), Rotation2d.fromDegrees(-180));

		startOrientation = Rotation2d.fromDegrees(0.0);
		endOrientation   = Rotation2d.fromDegrees(180);
	
		ArrayList<Translation2d> interiorWaypoints = new ArrayList<Translation2d>();
		interiorWaypoints.add(new Translation2d(5.0,1.0));
		interiorWaypoints.add(new Translation2d(5.0,1.0));
		interiorWaypoints.add(new Translation2d(7.0,5.0));
		interiorWaypoints.add(new Translation2d(9.0,3.0));
		interiorWaypoints.add(new Translation2d(7.0,1.0));	
		interiorWaypoints.add(new Translation2d(5.0,5.0));	
		
		double desTrajMaxSpeed = Constants.MAX_FWD_REV_SPEED_MPS * 0.65;
		double desTrajMaxAccel = Constants.MAX_TRANSLATE_ACCEL_MPS2 * 1.0;
		// End Config
		//////////////////////////////////////////////////////


		config = new TrajectoryConfig(desTrajMaxSpeed, desTrajMaxAccel);
		config.setKinematics(Constants.m_kinematics);
		config.addConstraint(new SwerveDriveKinematicsConstraint(Constants.m_kinematics, desTrajMaxSpeed));
		traj = TrajectoryGenerator.generateTrajectory( startPose, interiorWaypoints, endPose, config);
	}

	@Override
	public void userStart() {
		startTime = Timer.getFPGATimestamp();
        done = false;
	}

	@Override
	public void userUpdate() {
		var deltaTime = Timer.getFPGATimestamp() - startTime;

		if(deltaTime < traj.getTotalTimeSeconds()){
			Trajectory.State dtCmd = traj.sample(deltaTime);
			DrivetrainControl.getInstance().setInputs(dtCmd, interpolateRotation(deltaTime/traj.getTotalTimeSeconds()));
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

	public Pose2d getStartPose(){
		return traj.getInitialPose();
	}


	private Rotation2d interpolateRotation(double frac){ //Linear interpolation
		double start = startOrientation.getDegrees();
		double end = endOrientation.getDegrees();
		return Rotation2d.fromDegrees(start * (1.0 - frac) + end * frac);
	}

}
