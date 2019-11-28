/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "CommandTestBase.h"
#include "frc2/command/ConditionalCommand.h"
#include "frc2/command/ParallelCommandGroup.h"
#include "frc2/command/ParallelDeadlineGroup.h"
#include "frc2/command/ParallelRaceGroup.h"
#include "frc2/command/SelectCommand.h"
#include "frc2/command/SequentialCommandGroup.h"

using namespace frc2;
class RobotDisabledCommandTest : public CommandTestBase {};

TEST_F(RobotDisabledCommandTest, RobotDisabledCommandCancelTest) {
  CommandScheduler scheduler = GetScheduler();

  MockCommand command({}, false, false);

  EXPECT_CALL(command, End(true));

  SetDSEnabled(true);

  scheduler.Schedule(&command);
  scheduler.Run();

  EXPECT_TRUE(scheduler.IsScheduled(&command));

  SetDSEnabled(false);

  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(RobotDisabledCommandTest, RunWhenDisabledTest) {
  CommandScheduler scheduler = GetScheduler();

  MockCommand command1;
  MockCommand command2;

  scheduler.Schedule(&command1);

  SetDSEnabled(false);

  scheduler.Run();

  scheduler.Schedule(&command2);

  EXPECT_TRUE(scheduler.IsScheduled(&command1));
  EXPECT_TRUE(scheduler.IsScheduled(&command2));
}

TEST_F(RobotDisabledCommandTest, SequentialGroupRunWhenDisabledTest) {
  CommandScheduler scheduler = GetScheduler();

  SequentialCommandGroup runWhenDisabled{MockCommand(), MockCommand()};
  SequentialCommandGroup dontRunWhenDisabled{MockCommand(),
                                             MockCommand({}, false, false)};

  SetDSEnabled(false);

  scheduler.Schedule(&runWhenDisabled);
  scheduler.Schedule(&dontRunWhenDisabled);

  EXPECT_TRUE(scheduler.IsScheduled(&runWhenDisabled));
  EXPECT_FALSE(scheduler.IsScheduled(&dontRunWhenDisabled));
}

TEST_F(RobotDisabledCommandTest, ParallelGroupRunWhenDisabledTest) {
  CommandScheduler scheduler = GetScheduler();

  ParallelCommandGroup runWhenDisabled{MockCommand(), MockCommand()};
  ParallelCommandGroup dontRunWhenDisabled{MockCommand(),
                                           MockCommand({}, false, false)};

  SetDSEnabled(false);

  scheduler.Schedule(&runWhenDisabled);
  scheduler.Schedule(&dontRunWhenDisabled);

  EXPECT_TRUE(scheduler.IsScheduled(&runWhenDisabled));
  EXPECT_FALSE(scheduler.IsScheduled(&dontRunWhenDisabled));
}

TEST_F(RobotDisabledCommandTest, ParallelRaceRunWhenDisabledTest) {
  CommandScheduler scheduler = GetScheduler();

  ParallelRaceGroup runWhenDisabled{MockCommand(), MockCommand()};
  ParallelRaceGroup dontRunWhenDisabled{MockCommand(),
                                        MockCommand({}, false, false)};

  SetDSEnabled(false);

  scheduler.Schedule(&runWhenDisabled);
  scheduler.Schedule(&dontRunWhenDisabled);

  EXPECT_TRUE(scheduler.IsScheduled(&runWhenDisabled));
  EXPECT_FALSE(scheduler.IsScheduled(&dontRunWhenDisabled));
}

TEST_F(RobotDisabledCommandTest, ParallelDeadlineRunWhenDisabledTest) {
  CommandScheduler scheduler = GetScheduler();

  ParallelDeadlineGroup runWhenDisabled{MockCommand(), MockCommand()};
  ParallelDeadlineGroup dontRunWhenDisabled{MockCommand(),
                                            MockCommand({}, false, false)};

  SetDSEnabled(false);

  scheduler.Schedule(&runWhenDisabled);
  scheduler.Schedule(&dontRunWhenDisabled);

  EXPECT_TRUE(scheduler.IsScheduled(&runWhenDisabled));
  EXPECT_FALSE(scheduler.IsScheduled(&dontRunWhenDisabled));
}

TEST_F(RobotDisabledCommandTest, ConditionalCommandRunWhenDisabledTest) {
  CommandScheduler scheduler = GetScheduler();

  ConditionalCommand runWhenDisabled{MockCommand(), MockCommand(),
                                     [] { return true; }};
  ConditionalCommand dontRunWhenDisabled{
      MockCommand(), MockCommand({}, false, false), [] { return true; }};

  SetDSEnabled(false);

  scheduler.Schedule(&runWhenDisabled);
  scheduler.Schedule(&dontRunWhenDisabled);

  EXPECT_TRUE(scheduler.IsScheduled(&runWhenDisabled));
  EXPECT_FALSE(scheduler.IsScheduled(&dontRunWhenDisabled));
}

TEST_F(RobotDisabledCommandTest, SelectCommandRunWhenDisabledTest) {
  CommandScheduler scheduler = GetScheduler();

  SelectCommand<int> runWhenDisabled{[] { return 1; },
                                     std::pair(1, MockCommand()),
                                     std::pair(1, MockCommand())};
  SelectCommand<int> dontRunWhenDisabled{
      [] { return 1; }, std::pair(1, MockCommand()),
      std::pair(1, MockCommand({}, false, false))};

  SetDSEnabled(false);

  scheduler.Schedule(&runWhenDisabled);
  scheduler.Schedule(&dontRunWhenDisabled);

  EXPECT_TRUE(scheduler.IsScheduled(&runWhenDisabled));
  EXPECT_FALSE(scheduler.IsScheduled(&dontRunWhenDisabled));
}
