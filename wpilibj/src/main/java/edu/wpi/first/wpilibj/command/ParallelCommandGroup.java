package edu.wpi.first.wpilibj.command;


import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class ParallelCommandGroup extends CommandGroupBase implements ICommand {

  //maps commands in this group to whether they are still running
  private final Map<ICommand, Boolean> m_commands = new HashMap<>();

  /**
   * Creates a new ParallelCommandGroup.  The given commands will be executed simultaneously.
   * The command group will finish when the last command finishes.  If the commandgroup is
   * interrupted, only the commands that are still running will be interrupted.
   *
   * @param commands the commands to include in this group.
   */
  public ParallelCommandGroup(ICommand... commands) {
    if (!Collections.disjoint(Set.of(commands), getGroupedCommands())) {
      throw new IllegalUseOfCommandException("Commands cannot be added to multiple CommandGroups");
    }

    registerGroupedCommands(commands);

    for (ICommand command : commands) {
      m_commands.put(command, true);
    }
  }

  @Override
  public void initialize() {
    for (ICommand command : m_commands.keySet()) {
      command.initialize();
      m_commands.replace(command, true);
    }
  }

  @Override
  public void execute() {
    for (ICommand command : m_commands.keySet()) {
      if (!m_commands.get(command)) {
        continue;
      }
      command.execute();
      if (command.isFinished()) {
        command.end();
        m_commands.replace(command, false);
      }
    }
  }

  @Override
  public void interrupted() {
    for (ICommand command : m_commands.keySet()) {
      if (m_commands.get(command)) {
        command.interrupted();
      }
    }
  }

  @Override
  public void end() {
  }

  @Override
  public boolean isFinished() {
    return !m_commands.values().contains(true);
  }

  @Override
  public Collection<Subsystem> getRequirements() {
    Collection<Subsystem> requirements = new HashSet<>();
    for (ICommand command : m_commands.keySet()) {
      requirements.addAll(command.getRequirements());
    }
    return requirements;
  }

  @Override
  public boolean getRunWhenDisabled() {
    boolean allRunWhenDisabled = true;
    for(ICommand command : m_commands.keySet()) {
      allRunWhenDisabled &= command.getRunWhenDisabled();
    }
    return allRunWhenDisabled;
  }
}
