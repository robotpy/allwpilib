/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.commands.paralleldeadlinegroup;

import edu.wpi.first.wpilibj2.command.InstantCommand;
import edu.wpi.first.wpilibj2.command.ParallelDeadlineGroup;

// NOTE:  Consider using this command inline, rather than writing a subclass.  For more
// information, see:
// https://docs.wpilib.org/en/stable/docs/software/commandbased/convenience-features.html
public class ReplaceMeParallelDeadlineGroup extends ParallelDeadlineGroup {
  /**
   * Creates a new ReplaceMeParallelDeadlineGroup.
   */
  public ReplaceMeParallelDeadlineGroup() {
    // Add your commands in the super() call.  Add the deadline first.
    super(
        new InstantCommand()
    );
  }
}
