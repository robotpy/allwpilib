package edu.wpi.first.wpilibj.examples.gyrostabilization;

import edu.wpi.first.wpilibj.GenericHID;
import edu.wpi.first.wpilibj.XboxController;
import edu.wpi.first.wpilibj.examples.gyrostabilization.subsystems.DriveSubsystem;
import edu.wpi.first.wpilibj.experimental.command.*;
import edu.wpi.first.wpilibj.experimental.controller.PIDController;

import static edu.wpi.first.wpilibj.XboxController.Button;
import static edu.wpi.first.wpilibj.examples.gyrostabilization.Constants.DriveConstants.*;
import static edu.wpi.first.wpilibj.examples.gyrostabilization.Constants.OIConstants.*;

/**
 * This class is where the bulk of the robot should be declared.  Since Command-based is a
 * "declarative" paradigm, very little robot logic should actually be handled in the {@link Robot}
 * periodic methods (other than the scheduler calls).  Instead, the structure of the robot
 * (including subsystems, commands, and button mappings) should be declared here.
 */
public class RobotContainer {

  // The robot's subsystems
  private DriveSubsystem m_robotDrive = new DriveSubsystem();

  // The driver's controller
  XboxController driverController = new XboxController(kDriverControllerPort);

  public RobotContainer() {
    // Configure the button bindings
    configureButtonBindings();

    // Configure default commands
    // Set the default drive command to split-stick arcade drive
    m_robotDrive.setDefaultCommand(
        // A split-stick arcade command, with forward/backward controlled by the left
        // hand, and turning controlled by the right.
        new RunCommand(() -> m_robotDrive.arcadeDrive(
            driverController.getY(GenericHID.Hand.kLeft),
            driverController.getX(GenericHID.Hand.kRight)),
            m_robotDrive)
    );

  }

  /**
   * Use this method to define your button->command mappings.  Buttons can be created by
   * instantiating a {@link GenericHID} or one of its subclasses ({@link
   * edu.wpi.first.wpilibj.Joystick} or {@link XboxController}), and then calling {@link
   * GenericHID#getButton(int)}.
   */
  private void configureButtonBindings() {
    // Drive at half speed when the right bumper is held
    driverController.getButton(Button.kBumperRight.value)
        .whenPressed(() -> m_robotDrive.setMaxOutput(.5))
        .whenReleased(() -> m_robotDrive.setMaxOutput(1));

    // Stabilize robot to drive straight with gyro when left bumper is held
    driverController.getButton(Button.kBumperLeft.value)
        .whenHeld(new SynchronousPIDCommand(
                new PIDController(kStabilizationP, kStabilizationI, kStabilizationD),
                // Close the loop on the turn rate
                // Reference (setpoint) is implicitly 0
                m_robotDrive::getTurnRate,
                // Pipe the output to the turning controls
                (output) ->
                    m_robotDrive.arcadeDrive(driverController.getY(GenericHID.Hand.kLeft), output),
                // Require the robot drive
                m_robotDrive
            )
        );
  }


  /**
   * Use this to pass the autonomous command to the main {@link Robot} class.
   *
   * @return the command to run in autonomous
   */
  public Command getAutonomousCommand() {
    // no auto
    return new InstantCommand();
  }
}
