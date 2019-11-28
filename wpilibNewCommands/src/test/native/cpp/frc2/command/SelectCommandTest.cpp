/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "CommandTestBase.h"
#include "frc2/command/ConditionalCommand.h"
#include "frc2/command/InstantCommand.h"
#include "frc2/command/SelectCommand.h"

using namespace frc2;
class SelectCommandTest : public CommandTestBase {};

TEST_F(SelectCommandTest, SelectCommandTest) {
  CommandScheduler scheduler = GetScheduler();

  std::unique_ptr<MockCommand> mock = std::make_unique<MockCommand>();
  MockCommand* mockptr = mock.get();

  EXPECT_CALL(*mock, Initialize());
  EXPECT_CALL(*mock, Execute()).Times(2);
  EXPECT_CALL(*mock, End(false));

  std::vector<std::pair<int, std::unique_ptr<Command>>> temp;

  temp.emplace_back(std::pair(1, std::move(mock)));
  temp.emplace_back(std::pair(2, std::make_unique<InstantCommand>()));
  temp.emplace_back(std::pair(3, std::make_unique<InstantCommand>()));

  SelectCommand<int> select([] { return 1; }, std::move(temp));

  scheduler.Schedule(&select);
  scheduler.Run();
  mockptr->SetFinished(true);
  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&select));
}

TEST_F(SelectCommandTest, SelectCommandRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem requirement1;
  TestSubsystem requirement2;
  TestSubsystem requirement3;
  TestSubsystem requirement4;

  InstantCommand command1([] {}, {&requirement1, &requirement2});
  InstantCommand command2([] {}, {&requirement3});
  InstantCommand command3([] {}, {&requirement3, &requirement4});

  SelectCommand<int> select([] { return 1; }, std::pair(1, std::move(command1)),
                            std::pair(2, std::move(command2)));

  scheduler.Schedule(&select);
  scheduler.Schedule(&command3);

  EXPECT_TRUE(scheduler.IsScheduled(&command3));
  EXPECT_FALSE(scheduler.IsScheduled(&select));
}
