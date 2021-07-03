/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot;

import edu.wpi.first.wpilibj.PowerDistributionPanel;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.Timer;
import frc.robot.HumanInterface.DriverInterface;
import frc.sim.RobotModel;
import frc.robot.Autonomous.Autonomous;
import frc.robot.Drivetrain.DrivetrainControl;
import frc.robot.Drivetrain.DrivetrainPoseEstimator;
import frc.PoseTelemetry;

/**
 * The VM is configured to automatically run this class, and to call the
 * functions corresponding to each mode, as described in the TimedRobot
 * documentation. If you change the name of this class or the package after
 * creating this project, you must also update the build.gradle file in the
 * project.
 */
public class Robot extends TimedRobot {

  DrivetrainControl dt;
  DrivetrainPoseEstimator dtpe;

  PoseTelemetry dtPoseView;

  DriverInterface di;

  PowerDistributionPanel pdp;

  Autonomous a;
  
  int loopCounter = 0;

  
  double curBatVoltage = 0;
  
  double curBatCurDraw = 0;

  /**
   * This function is run when the robot is first started up and should be used
   * for any initialization code.
   */
  @Override
  public void robotInit() {

    dtPoseView = new PoseTelemetry();

    dt = DrivetrainControl.getInstance();
    dtpe = DrivetrainPoseEstimator.getInstance();

    di = new DriverInterface();

    pdp = new PowerDistributionPanel();

    a = new Autonomous();


    if(isSimulation()){
      simModel = new RobotModel();
    }

  }

  /**
   * This function is called once as the robot enters autnonmous mode.
   */
  @Override
  public void autonomousInit() {

    //One final update to autonomous mode selection to make sure it's correct
    a.modeUpdate();

    //Reset simulation model to zero state.
    if(isSimulation()){
      simModel.reset(a.getStartPose());
    }

    dtpe.setKnownPose(a.getStartPose());

    a.start();
  }

  /**
   * This function is called periodically during autonomous.
   */
  @Override
  public void autonomousPeriodic() {
    a.sequencerUpdate();
    periodicCommon();
  }

  /**
   * This function is called once as the robot enters teleop mode
   */
  @Override
  public void teleopInit() {

  }

  /**
   * This function is called periodically during operator control.
   */
  @Override
  public void teleopPeriodic() {
    di.update();
    dt.setInputs(di.getFwdRevSpeedCmd_mps(), 
                 di.getStrafeSpeedCmd_mps(), 
                 di.getRotateCmd_radPerSec());

    periodicCommon();
  }

  /**
   * This function is called once as the robot enters disabled mode
   */
  @Override
  public void disabledInit() {
    
    a.stop();

  }

  /**
   * This function is called periodically when disabled.
   */
  @Override
  public void disabledPeriodic() {
    a.modeUpdate();
    periodicCommon();
  }


  void periodicCommon() {
    loopCounter++;
    dt.update();
    dtpe.update();

    updateTelemetry();

  }

  private void updateTelemetry(){
    
    if(isSimulation()){
      dtPoseView.setActualPose(simModel.getCurActPose());
    }
    dtPoseView.setEstimatedPose(dtpe.getEstPose());
    dtPoseView.setDesiredPose(dt.getCurDesiredPose());

    curBatVoltage = pdp.getVoltage();
    curBatCurDraw = pdp.getTotalCurrent();

    dtPoseView.update(Timer.getFPGATimestamp()*1000);
  }


  /*=========================================================================*/
  /*=========================================================================*/

  /*
   * This set of functions is for simulation only, and is not called on the real
   * robot. Put plant-model related functionality here. For training purposes,
   * students should not have to modify this functionality.
   */

  // Simple robot plant model for simulation purposes
  RobotModel simModel;

  @Override
  public void simulationInit() {
  }

  @Override
  public void simulationPeriodic() {

    simModel.update(isDisabled());
  }

  /*=========================================================================*/
  /*=========================================================================*/

}
