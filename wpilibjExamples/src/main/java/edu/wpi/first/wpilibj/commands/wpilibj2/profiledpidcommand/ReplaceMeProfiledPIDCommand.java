/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.commands.wpilibj2.profiledpidcommand;

import edu.wpi.first.wpilibj.controller.ProfiledPIDController;
import edu.wpi.first.wpilibj.trajectory.TrapezoidProfile;
import edu.wpi.first.wpilibj2.command.ProfiledPIDCommand;

// NOTE:  Consider using this command inline, rather than writing a subclass.  For more
// information, see:
// https://docs.wpilib.org/en/latest/docs/software/commandbased/convenience-features.html
public class ReplaceMeProfiledPIDCommand extends ProfiledPIDCommand {
  public ReplaceMeProfiledPIDCommand() {
    super(new ProfiledPIDController(0, 0, 0,
                                    new TrapezoidProfile.Constraints(0, 0)),
          // This should return the measurement
          () -> 0,
          // This should return the goal (can also be a constant)
          () -> new TrapezoidProfile.State(),
          // This uses the output
          (output, setpoint) -> {
            // Use the output (and setpoint, if desired) here
          });
    // Use addRequirements() here to declare subsystem dependencies.
    // Configure additional PID options by calling `getController` here.
  }

  // Returns true when the command should end.
  @Override
  public boolean isFinished() {
    return false;
  }
}
