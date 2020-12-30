// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj;

import static org.junit.jupiter.api.Assertions.assertEquals;

import edu.wpi.first.wpilibj.simulation.DriverStationSim;
import edu.wpi.first.wpilibj.simulation.SimHooks;
import java.util.concurrent.atomic.AtomicInteger;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.parallel.ResourceLock;

class TimedRobotTest {
  class MockRobot extends TimedRobot {
    public final AtomicInteger m_robotInitCount = new AtomicInteger(0);
    public final AtomicInteger m_simulationInitCount = new AtomicInteger(0);
    public final AtomicInteger m_disabledInitCount = new AtomicInteger(0);
    public final AtomicInteger m_autonomousInitCount = new AtomicInteger(0);
    public final AtomicInteger m_teleopInitCount = new AtomicInteger(0);
    public final AtomicInteger m_testInitCount = new AtomicInteger(0);

    public final AtomicInteger m_robotPeriodicCount = new AtomicInteger(0);
    public final AtomicInteger m_simulationPeriodicCount = new AtomicInteger(0);
    public final AtomicInteger m_disabledPeriodicCount = new AtomicInteger(0);
    public final AtomicInteger m_autonomousPeriodicCount = new AtomicInteger(0);
    public final AtomicInteger m_teleopPeriodicCount = new AtomicInteger(0);
    public final AtomicInteger m_testPeriodicCount = new AtomicInteger(0);

    @Override
    public void robotInit() {
      m_robotInitCount.addAndGet(1);
    }

    @Override
    public void simulationInit() {
      m_simulationInitCount.addAndGet(1);
    }

    @Override
    public void disabledInit() {
      m_disabledInitCount.addAndGet(1);
    }

    @Override
    public void autonomousInit() {
      m_autonomousInitCount.addAndGet(1);
    }

    @Override
    public void teleopInit() {
      m_teleopInitCount.addAndGet(1);
    }

    @Override
    public void testInit() {
      m_testInitCount.addAndGet(1);
    }

    @Override
    public void robotPeriodic() {
      m_robotPeriodicCount.addAndGet(1);
    }

    @Override
    public void simulationPeriodic() {
      m_simulationPeriodicCount.addAndGet(1);
    }

    @Override
    public void disabledPeriodic() {
      m_disabledPeriodicCount.addAndGet(1);
    }

    @Override
    public void autonomousPeriodic() {
      m_autonomousPeriodicCount.addAndGet(1);
    }

    @Override
    public void teleopPeriodic() {
      m_teleopPeriodicCount.addAndGet(1);
    }

    @Override
    public void testPeriodic() {
      m_testPeriodicCount.addAndGet(1);
    }
  }

  @BeforeEach
  void setup() {
    SimHooks.pauseTiming();
  }

  @AfterEach
  void cleanup() {
    SimHooks.resumeTiming();
  }

  @Test
  @ResourceLock("timing")
  void disabledTest() {
    MockRobot robot = new MockRobot();

    Thread robotThread =
        new Thread(
            () -> {
              robot.startCompetition();
            });
    robotThread.start();

    DriverStationSim.setEnabled(false);
    DriverStationSim.notifyNewData();
    SimHooks.stepTiming(0.0); // Wait for Notifiers

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_autonomousInitCount.get());
    assertEquals(0, robot.m_teleopInitCount.get());
    assertEquals(0, robot.m_testInitCount.get());

    assertEquals(0, robot.m_robotPeriodicCount.get());
    assertEquals(0, robot.m_simulationPeriodicCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(0, robot.m_autonomousPeriodicCount.get());
    assertEquals(0, robot.m_teleopPeriodicCount.get());
    assertEquals(0, robot.m_testPeriodicCount.get());

    SimHooks.stepTiming(0.02);

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(1, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_autonomousInitCount.get());
    assertEquals(0, robot.m_teleopInitCount.get());
    assertEquals(0, robot.m_testInitCount.get());

    assertEquals(1, robot.m_robotPeriodicCount.get());
    assertEquals(1, robot.m_simulationPeriodicCount.get());
    assertEquals(1, robot.m_disabledPeriodicCount.get());
    assertEquals(0, robot.m_autonomousPeriodicCount.get());
    assertEquals(0, robot.m_teleopPeriodicCount.get());
    assertEquals(0, robot.m_testPeriodicCount.get());

    SimHooks.stepTiming(0.02);

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(1, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_autonomousInitCount.get());
    assertEquals(0, robot.m_teleopInitCount.get());
    assertEquals(0, robot.m_testInitCount.get());

    assertEquals(2, robot.m_robotPeriodicCount.get());
    assertEquals(2, robot.m_simulationPeriodicCount.get());
    assertEquals(2, robot.m_disabledPeriodicCount.get());
    assertEquals(0, robot.m_autonomousPeriodicCount.get());
    assertEquals(0, robot.m_teleopPeriodicCount.get());
    assertEquals(0, robot.m_testPeriodicCount.get());

    robot.endCompetition();
    try {
      robotThread.interrupt();
      robotThread.join();
    } catch (InterruptedException ex) {
      Thread.currentThread().interrupt();
    }
    robot.close();
  }

  @Test
  @ResourceLock("timing")
  void autonomousTest() {
    MockRobot robot = new MockRobot();

    Thread robotThread =
        new Thread(
            () -> {
              robot.startCompetition();
            });
    robotThread.start();

    DriverStationSim.setEnabled(true);
    DriverStationSim.setAutonomous(true);
    DriverStationSim.setTest(false);
    DriverStationSim.notifyNewData();
    SimHooks.stepTiming(0.0); // Wait for Notifiers

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_autonomousInitCount.get());
    assertEquals(0, robot.m_teleopInitCount.get());
    assertEquals(0, robot.m_testInitCount.get());

    assertEquals(0, robot.m_robotPeriodicCount.get());
    assertEquals(0, robot.m_simulationPeriodicCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(0, robot.m_autonomousPeriodicCount.get());
    assertEquals(0, robot.m_teleopPeriodicCount.get());
    assertEquals(0, robot.m_testPeriodicCount.get());

    SimHooks.stepTiming(0.02);

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(1, robot.m_autonomousInitCount.get());
    assertEquals(0, robot.m_teleopInitCount.get());
    assertEquals(0, robot.m_testInitCount.get());

    assertEquals(1, robot.m_robotPeriodicCount.get());
    assertEquals(1, robot.m_simulationPeriodicCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(1, robot.m_autonomousPeriodicCount.get());
    assertEquals(0, robot.m_teleopPeriodicCount.get());
    assertEquals(0, robot.m_testPeriodicCount.get());

    SimHooks.stepTiming(0.02);

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(1, robot.m_autonomousInitCount.get());
    assertEquals(0, robot.m_teleopInitCount.get());
    assertEquals(0, robot.m_testInitCount.get());

    assertEquals(2, robot.m_robotPeriodicCount.get());
    assertEquals(2, robot.m_simulationPeriodicCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(2, robot.m_autonomousPeriodicCount.get());
    assertEquals(0, robot.m_teleopPeriodicCount.get());
    assertEquals(0, robot.m_testPeriodicCount.get());

    robot.endCompetition();
    try {
      robotThread.interrupt();
      robotThread.join();
    } catch (InterruptedException ex) {
      Thread.currentThread().interrupt();
    }
    robot.close();
  }

  @Test
  @ResourceLock("timing")
  void teleopTest() {
    MockRobot robot = new MockRobot();

    Thread robotThread =
        new Thread(
            () -> {
              robot.startCompetition();
            });
    robotThread.start();

    DriverStationSim.setEnabled(true);
    DriverStationSim.setAutonomous(false);
    DriverStationSim.setTest(false);
    DriverStationSim.notifyNewData();
    SimHooks.stepTiming(0.0); // Wait for Notifiers

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_autonomousInitCount.get());
    assertEquals(0, robot.m_teleopInitCount.get());
    assertEquals(0, robot.m_testInitCount.get());

    assertEquals(0, robot.m_robotPeriodicCount.get());
    assertEquals(0, robot.m_simulationPeriodicCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(0, robot.m_autonomousPeriodicCount.get());
    assertEquals(0, robot.m_teleopPeriodicCount.get());
    assertEquals(0, robot.m_testPeriodicCount.get());

    SimHooks.stepTiming(0.02);

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_autonomousInitCount.get());
    assertEquals(1, robot.m_teleopInitCount.get());
    assertEquals(0, robot.m_testInitCount.get());

    assertEquals(1, robot.m_robotPeriodicCount.get());
    assertEquals(1, robot.m_simulationPeriodicCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(0, robot.m_autonomousPeriodicCount.get());
    assertEquals(1, robot.m_teleopPeriodicCount.get());
    assertEquals(0, robot.m_testPeriodicCount.get());

    SimHooks.stepTiming(0.02);

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_autonomousInitCount.get());
    assertEquals(1, robot.m_teleopInitCount.get());
    assertEquals(0, robot.m_testInitCount.get());

    assertEquals(2, robot.m_robotPeriodicCount.get());
    assertEquals(2, robot.m_simulationPeriodicCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(0, robot.m_autonomousPeriodicCount.get());
    assertEquals(2, robot.m_teleopPeriodicCount.get());
    assertEquals(0, robot.m_testPeriodicCount.get());

    robot.endCompetition();
    try {
      robotThread.interrupt();
      robotThread.join();
    } catch (InterruptedException ex) {
      Thread.currentThread().interrupt();
    }
    robot.close();
  }

  @Test
  @ResourceLock("timing")
  void testTest() {
    MockRobot robot = new MockRobot();

    Thread robotThread =
        new Thread(
            () -> {
              robot.startCompetition();
            });
    robotThread.start();

    DriverStationSim.setEnabled(true);
    DriverStationSim.setAutonomous(false);
    DriverStationSim.setTest(true);
    DriverStationSim.notifyNewData();
    SimHooks.stepTiming(0.0); // Wait for Notifiers

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_autonomousInitCount.get());
    assertEquals(0, robot.m_teleopInitCount.get());
    assertEquals(0, robot.m_testInitCount.get());

    assertEquals(0, robot.m_robotPeriodicCount.get());
    assertEquals(0, robot.m_simulationPeriodicCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(0, robot.m_autonomousPeriodicCount.get());
    assertEquals(0, robot.m_teleopPeriodicCount.get());
    assertEquals(0, robot.m_testPeriodicCount.get());

    SimHooks.stepTiming(0.02);

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_autonomousInitCount.get());
    assertEquals(0, robot.m_teleopInitCount.get());
    assertEquals(1, robot.m_testInitCount.get());

    assertEquals(1, robot.m_robotPeriodicCount.get());
    assertEquals(1, robot.m_simulationPeriodicCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(0, robot.m_autonomousPeriodicCount.get());
    assertEquals(0, robot.m_teleopPeriodicCount.get());
    assertEquals(1, robot.m_testPeriodicCount.get());

    SimHooks.stepTiming(0.02);

    assertEquals(1, robot.m_robotInitCount.get());
    assertEquals(1, robot.m_simulationInitCount.get());
    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_autonomousInitCount.get());
    assertEquals(0, robot.m_teleopInitCount.get());
    assertEquals(1, robot.m_testInitCount.get());

    assertEquals(2, robot.m_robotPeriodicCount.get());
    assertEquals(2, robot.m_simulationPeriodicCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(0, robot.m_autonomousPeriodicCount.get());
    assertEquals(0, robot.m_teleopPeriodicCount.get());
    assertEquals(2, robot.m_testPeriodicCount.get());

    robot.endCompetition();
    try {
      robotThread.interrupt();
      robotThread.join();
    } catch (InterruptedException ex) {
      Thread.currentThread().interrupt();
    }
    robot.close();
  }

  @Test
  @ResourceLock("timing")
  void addPeriodicTest() {
    MockRobot robot = new MockRobot();

    final AtomicInteger callbackCount = new AtomicInteger(0);
    robot.addPeriodic(
        () -> {
          callbackCount.addAndGet(1);
        },
        0.01);

    Thread robotThread =
        new Thread(
            () -> {
              robot.startCompetition();
            });
    robotThread.start();

    DriverStationSim.setEnabled(false);
    DriverStationSim.notifyNewData();
    SimHooks.stepTiming(0.0); // Wait for Notifiers

    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(0, callbackCount.get());

    SimHooks.stepTiming(0.01);

    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(1, callbackCount.get());

    SimHooks.stepTiming(0.01);

    assertEquals(1, robot.m_disabledInitCount.get());
    assertEquals(1, robot.m_disabledPeriodicCount.get());
    assertEquals(2, callbackCount.get());

    robot.endCompetition();
    try {
      robotThread.interrupt();
      robotThread.join();
    } catch (InterruptedException ex) {
      Thread.currentThread().interrupt();
    }
    robot.close();
  }

  @Test
  @ResourceLock("timing")
  void addPeriodicWithOffsetTest() {
    MockRobot robot = new MockRobot();

    final AtomicInteger callbackCount = new AtomicInteger(0);
    robot.addPeriodic(
        () -> {
          callbackCount.addAndGet(1);
        },
        0.01,
        0.005);

    // Expirations in this test (ms)
    //
    // Robot | Callback
    // ================
    //    20 |      15
    //    40 |      25

    Thread robotThread =
        new Thread(
            () -> {
              robot.startCompetition();
            });
    robotThread.start();

    DriverStationSim.setEnabled(false);
    DriverStationSim.notifyNewData();
    SimHooks.stepTiming(0.0); // Wait for Notifiers

    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(0, callbackCount.get());

    SimHooks.stepTiming(0.0075);

    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(0, callbackCount.get());

    SimHooks.stepTiming(0.0075);

    assertEquals(0, robot.m_disabledInitCount.get());
    assertEquals(0, robot.m_disabledPeriodicCount.get());
    assertEquals(1, callbackCount.get());

    SimHooks.stepTiming(0.005);

    assertEquals(1, robot.m_disabledInitCount.get());
    assertEquals(1, robot.m_disabledPeriodicCount.get());
    assertEquals(1, callbackCount.get());

    SimHooks.stepTiming(0.005);

    assertEquals(1, robot.m_disabledInitCount.get());
    assertEquals(1, robot.m_disabledPeriodicCount.get());
    assertEquals(2, callbackCount.get());

    robot.endCompetition();
    try {
      robotThread.interrupt();
      robotThread.join();
    } catch (InterruptedException ex) {
      Thread.currentThread().interrupt();
    }
    robot.close();
  }
}
