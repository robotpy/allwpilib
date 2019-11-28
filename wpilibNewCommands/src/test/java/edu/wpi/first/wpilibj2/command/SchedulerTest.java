/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj2.command;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;

class SchedulerTest extends CommandTestBase {
  @Test
  void schedulerLambdaTestNoInterrupt() {
    CommandScheduler scheduler = new CommandScheduler();

    Counter counter = new Counter();

    scheduler.onCommandInitialize(command -> counter.increment());
    scheduler.onCommandExecute(command -> counter.increment());
    scheduler.onCommandFinish(command -> counter.increment());

    scheduler.schedule(new InstantCommand());
    scheduler.run();

    assertEquals(counter.m_counter, 3);
  }

  @Test
  void schedulerInterruptLambdaTest() {
    CommandScheduler scheduler = new CommandScheduler();

    Counter counter = new Counter();

    scheduler.onCommandInterrupt(command -> counter.increment());

    Command command = new WaitCommand(10);

    scheduler.schedule(command);
    scheduler.cancel(command);

    assertEquals(counter.m_counter, 1);
  }

  @Test
  void unregisterSubsystemTest() {
    CommandScheduler scheduler = new CommandScheduler();

    Subsystem system = new TestSubsystem();

    scheduler.registerSubsystem(system);
    assertDoesNotThrow(() -> scheduler.unregisterSubsystem(system));
  }
}
