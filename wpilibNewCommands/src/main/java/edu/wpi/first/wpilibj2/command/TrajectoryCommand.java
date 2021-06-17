// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj2.command;

import edu.wpi.first.math.trajectory.Trajectory;
import edu.wpi.first.wpilibj.Timer;
import java.util.function.Consumer;

/**
 * A command that follows a certain trajectory, passing the target state to an output function every
 * iteration.
 *
 * <p>This command does <b>not</b> handle Ramsete, PID, and Feedforward control internally - they
 * should be used by the subsystem in the output function.
 */
public class TrajectoryCommand extends CommandBase {
  private final Trajectory m_trajectory;
  private final Consumer<Trajectory.State> m_output;
  private final Timer m_timer = new Timer();
  private double m_prevTime;

  /**
   * Create a new TrajectoryCommand.
   *
   * @param trajectory the trajectory to follow.
   * @param output a function that consumes the target state for each timestep. This state can be
   *     fed into a Ramsete controller or similar.
   * @param requirements the subsystems to require.
   */
  public TrajectoryCommand(
      Trajectory trajectory, Consumer<Trajectory.State> output, Subsystem... requirements) {
    m_trajectory = trajectory;
    m_output = output;

    addRequirements(requirements);
  }

  @Override
  public void initialize() {
    m_prevTime = -1;
    m_timer.reset();
    m_timer.start();
  }

  @Override
  public void execute() {
    double curTime = m_timer.get();

    if (m_prevTime < 0) {
      m_output.accept(new Trajectory.State());
      m_prevTime = curTime;
      return;
    }

    m_output.accept(m_trajectory.sample(curTime));

    m_prevTime = curTime;
  }

  @Override
  public boolean isFinished() {
    return m_timer.hasElapsed(m_trajectory.getTotalTimeSeconds());
  }

  @Override
  public void end(boolean interrupted) {
    m_timer.stop();

    if (interrupted) {
      m_output.accept(new Trajectory.State());
    }
  }
}
