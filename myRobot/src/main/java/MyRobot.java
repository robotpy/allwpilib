/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

import edu.wpi.first.wpilibj.CounterBase;
import edu.wpi.first.wpilibj.Encoder;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.simulation.Assembly2D;

@SuppressWarnings("all")
public class MyRobot extends TimedRobot {

    int counter = 0;
    Assembly2D assembly2D = new Assembly2D();
  /**
     * This function is run when the robot is first started up and should be
     * used for any initialization code.
     */
    private final Encoder m_encoder = new Encoder(1, 2, false, CounterBase.EncodingType.k4X);
  @Override
  public void robotInit() {}

  /**
     * This function is run once each time the robot enters autonomous mode
     */
  @Override
  public void autonomousInit() {}

  /**
     * This function is called periodically during autonomous
     */
  @Override
  public void autonomousPeriodic() {}

  /**
     * This function is called once each time the robot enters tele-operated mode
     */
  @Override
  public void teleopInit() {}

  /**
     * This function is called periodically during operator control
     */
  @Override
  public void teleopPeriodic() {}

  /**
     * This function is called periodically during test mode
     */
  @Override
  public void testPeriodic() {}

  /**
     * This function is called periodically during all modes
     */
  @Override
  public void robotPeriodic() {
  assembly2D.setLigamentAngle("/JasonOne/one/", counter++);
  if(counter > 90){
    counter = -90;
  }
  assembly2D.setLigamentAngle("/JasonOne/one/two/", counter / 4);
  assembly2D.setLigamentLength("/JasonOne/one/two/", counter);
  }
}
