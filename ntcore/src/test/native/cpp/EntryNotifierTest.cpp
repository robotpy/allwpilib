/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <support/Logger.h>

#include "EntryNotifier.h"
#include "TestPrinters.h"
#include "ValueMatcher.h"
#include "gtest/gtest.h"

using ::testing::AnyNumber;
using ::testing::IsNull;
using ::testing::Return;
using ::testing::_;

namespace nt {

class EntryNotifierTest : public ::testing::Test {
 public:
  EntryNotifierTest() : notifier(1, logger) { notifier.Start(); }

  void GenerateNotifications();

 protected:
  wpi::Logger logger;
  EntryNotifier notifier;
};

void EntryNotifierTest::GenerateNotifications() {
  // All flags combos that can be generated by Storage
  static const unsigned int flags[] = {
      // "normal" notifications
      NT_NOTIFY_NEW, NT_NOTIFY_DELETE, NT_NOTIFY_UPDATE, NT_NOTIFY_FLAGS,
      NT_NOTIFY_UPDATE | NT_NOTIFY_FLAGS,
      // immediate notifications are always "new"
      NT_NOTIFY_IMMEDIATE | NT_NOTIFY_NEW,
      // local notifications can be of any flag combo
      NT_NOTIFY_LOCAL | NT_NOTIFY_NEW, NT_NOTIFY_LOCAL | NT_NOTIFY_DELETE,
      NT_NOTIFY_LOCAL | NT_NOTIFY_UPDATE, NT_NOTIFY_LOCAL | NT_NOTIFY_FLAGS,
      NT_NOTIFY_LOCAL | NT_NOTIFY_UPDATE | NT_NOTIFY_FLAGS};
  // Generate across keys
  static const char* keys[] = {"/foo/bar", "/baz", "/boo"};

  auto val = Value::MakeDouble(1);

  // Provide unique key indexes for each key
  unsigned int keyindex = 5;
  for (auto key : keys) {
    for (auto flag : flags) {
      notifier.NotifyEntry(keyindex, key, val, flag);
    }
    ++keyindex;
  }
}

TEST_F(EntryNotifierTest, PollEntryMultiple) {
  auto poller1 = notifier.CreatePoller();
  auto poller2 = notifier.CreatePoller();
  auto poller3 = notifier.CreatePoller();
  auto h1 = notifier.AddPolled(poller1, 6, NT_NOTIFY_NEW);
  auto h2 = notifier.AddPolled(poller2, 6, NT_NOTIFY_NEW);
  auto h3 = notifier.AddPolled(poller3, 6, NT_NOTIFY_UPDATE);

  ASSERT_FALSE(notifier.local_notifiers());

  GenerateNotifications();

  ASSERT_TRUE(notifier.WaitForQueue(1.0));
  bool timed_out = false;
  auto results1 = notifier.Poll(poller1, 0, &timed_out);
  ASSERT_FALSE(timed_out);
  auto results2 = notifier.Poll(poller2, 0, &timed_out);
  ASSERT_FALSE(timed_out);
  auto results3 = notifier.Poll(poller3, 0, &timed_out);
  ASSERT_FALSE(timed_out);

  ASSERT_EQ(results1.size(), 2u);
  for (const auto& result : results1) {
    SCOPED_TRACE(::testing::PrintToString(result));
    EXPECT_EQ(Handle{result.listener}.GetIndex(), (int)h1);
  }

  ASSERT_EQ(results2.size(), 2u);
  for (const auto& result : results2) {
    SCOPED_TRACE(::testing::PrintToString(result));
    EXPECT_EQ(Handle{result.listener}.GetIndex(), (int)h2);
  }

  ASSERT_EQ(results3.size(), 2u);
  for (const auto& result : results3) {
    SCOPED_TRACE(::testing::PrintToString(result));
    EXPECT_EQ(Handle{result.listener}.GetIndex(), (int)h3);
  }
}

TEST_F(EntryNotifierTest, PollEntryBasic) {
  auto poller = notifier.CreatePoller();
  auto g1 = notifier.AddPolled(poller, 6, NT_NOTIFY_NEW);
  auto g2 = notifier.AddPolled(poller, 6, NT_NOTIFY_DELETE);
  auto g3 = notifier.AddPolled(poller, 6, NT_NOTIFY_UPDATE);
  auto g4 = notifier.AddPolled(poller, 6, NT_NOTIFY_FLAGS);

  ASSERT_FALSE(notifier.local_notifiers());

  GenerateNotifications();

  ASSERT_TRUE(notifier.WaitForQueue(1.0));
  bool timed_out = false;
  auto results = notifier.Poll(poller, 0, &timed_out);
  ASSERT_FALSE(timed_out);

  int g1count = 0;
  int g2count = 0;
  int g3count = 0;
  int g4count = 0;
  for (const auto& result : results) {
    SCOPED_TRACE(::testing::PrintToString(result));
    EXPECT_EQ(result.name, "/baz");
    EXPECT_THAT(result.value, ValueEq(Value::MakeDouble(1)));
    EXPECT_EQ(Handle{result.entry}.GetType(), Handle::kEntry);
    EXPECT_EQ(Handle{result.entry}.GetInst(), 1);
    EXPECT_EQ(Handle{result.entry}.GetIndex(), 6);
    EXPECT_EQ(Handle{result.listener}.GetType(), Handle::kEntryListener);
    EXPECT_EQ(Handle{result.listener}.GetInst(), 1);
    if (Handle{result.listener}.GetIndex() == static_cast<int>(g1)) {
      ++g1count;
      EXPECT_TRUE((result.flags & NT_NOTIFY_NEW) != 0);
    } else if (Handle{result.listener}.GetIndex() == static_cast<int>(g2)) {
      ++g2count;
      EXPECT_TRUE((result.flags & NT_NOTIFY_DELETE) != 0);
    } else if (Handle{result.listener}.GetIndex() == static_cast<int>(g3)) {
      ++g3count;
      EXPECT_TRUE((result.flags & NT_NOTIFY_UPDATE) != 0);
    } else if (Handle{result.listener}.GetIndex() == static_cast<int>(g4)) {
      ++g4count;
      EXPECT_TRUE((result.flags & NT_NOTIFY_FLAGS) != 0);
    } else {
      ADD_FAILURE() << "unknown listener index";
    }
  }
  EXPECT_EQ(g1count, 2);
  EXPECT_EQ(g2count, 1);  // NT_NOTIFY_DELETE
  EXPECT_EQ(g3count, 2);
  EXPECT_EQ(g4count, 2);
}

TEST_F(EntryNotifierTest, PollEntryImmediate) {
  auto poller = notifier.CreatePoller();
  notifier.AddPolled(poller, 6, NT_NOTIFY_NEW | NT_NOTIFY_IMMEDIATE);
  notifier.AddPolled(poller, 6, NT_NOTIFY_NEW);

  ASSERT_FALSE(notifier.local_notifiers());

  GenerateNotifications();

  ASSERT_TRUE(notifier.WaitForQueue(1.0));
  bool timed_out = false;
  auto results = notifier.Poll(poller, 0, &timed_out);
  ASSERT_FALSE(timed_out);
  SCOPED_TRACE(::testing::PrintToString(results));
  ASSERT_EQ(results.size(), 4u);
}

TEST_F(EntryNotifierTest, PollEntryLocal) {
  auto poller = notifier.CreatePoller();
  notifier.AddPolled(poller, 6, NT_NOTIFY_NEW | NT_NOTIFY_LOCAL);
  notifier.AddPolled(poller, 6, NT_NOTIFY_NEW);

  ASSERT_TRUE(notifier.local_notifiers());

  GenerateNotifications();

  ASSERT_TRUE(notifier.WaitForQueue(1.0));
  bool timed_out = false;
  auto results = notifier.Poll(poller, 0, &timed_out);
  ASSERT_FALSE(timed_out);
  SCOPED_TRACE(::testing::PrintToString(results));
  ASSERT_EQ(results.size(), 6u);
}

TEST_F(EntryNotifierTest, PollPrefixMultiple) {
  auto poller1 = notifier.CreatePoller();
  auto poller2 = notifier.CreatePoller();
  auto poller3 = notifier.CreatePoller();
  auto h1 = notifier.AddPolled(poller1, "/foo", NT_NOTIFY_NEW);
  auto h2 = notifier.AddPolled(poller2, "/foo", NT_NOTIFY_NEW);
  auto h3 = notifier.AddPolled(poller3, "/foo", NT_NOTIFY_UPDATE);

  ASSERT_FALSE(notifier.local_notifiers());

  GenerateNotifications();

  ASSERT_TRUE(notifier.WaitForQueue(1.0));
  bool timed_out = false;
  auto results1 = notifier.Poll(poller1, 0, &timed_out);
  ASSERT_FALSE(timed_out);
  auto results2 = notifier.Poll(poller2, 0, &timed_out);
  ASSERT_FALSE(timed_out);
  auto results3 = notifier.Poll(poller3, 0, &timed_out);
  ASSERT_FALSE(timed_out);

  ASSERT_EQ(results1.size(), 2u);
  for (const auto& result : results1) {
    SCOPED_TRACE(::testing::PrintToString(result));
    EXPECT_EQ(Handle{result.listener}.GetIndex(), (int)h1);
  }

  ASSERT_EQ(results2.size(), 2u);
  for (const auto& result : results2) {
    SCOPED_TRACE(::testing::PrintToString(result));
    EXPECT_EQ(Handle{result.listener}.GetIndex(), (int)h2);
  }

  ASSERT_EQ(results3.size(), 2u);
  for (const auto& result : results3) {
    SCOPED_TRACE(::testing::PrintToString(result));
    EXPECT_EQ(Handle{result.listener}.GetIndex(), (int)h3);
  }
}

TEST_F(EntryNotifierTest, PollPrefixBasic) {
  auto poller = notifier.CreatePoller();
  auto g1 = notifier.AddPolled(poller, "/foo", NT_NOTIFY_NEW);
  auto g2 = notifier.AddPolled(poller, "/foo", NT_NOTIFY_DELETE);
  auto g3 = notifier.AddPolled(poller, "/foo", NT_NOTIFY_UPDATE);
  auto g4 = notifier.AddPolled(poller, "/foo", NT_NOTIFY_FLAGS);
  notifier.AddPolled(poller, "/bar", NT_NOTIFY_NEW);
  notifier.AddPolled(poller, "/bar", NT_NOTIFY_DELETE);
  notifier.AddPolled(poller, "/bar", NT_NOTIFY_UPDATE);
  notifier.AddPolled(poller, "/bar", NT_NOTIFY_FLAGS);

  ASSERT_FALSE(notifier.local_notifiers());

  GenerateNotifications();

  ASSERT_TRUE(notifier.WaitForQueue(1.0));
  bool timed_out = false;
  auto results = notifier.Poll(poller, 0, &timed_out);
  ASSERT_FALSE(timed_out);

  int g1count = 0;
  int g2count = 0;
  int g3count = 0;
  int g4count = 0;
  for (const auto& result : results) {
    SCOPED_TRACE(::testing::PrintToString(result));
    EXPECT_TRUE(StringRef(result.name).startswith("/foo"));
    EXPECT_THAT(result.value, ValueEq(Value::MakeDouble(1)));
    EXPECT_EQ(Handle{result.entry}.GetType(), Handle::kEntry);
    EXPECT_EQ(Handle{result.entry}.GetInst(), 1);
    EXPECT_EQ(Handle{result.entry}.GetIndex(), 5);
    EXPECT_EQ(Handle{result.listener}.GetType(), Handle::kEntryListener);
    EXPECT_EQ(Handle{result.listener}.GetInst(), 1);
    if (Handle{result.listener}.GetIndex() == static_cast<int>(g1)) {
      ++g1count;
      EXPECT_TRUE((result.flags & NT_NOTIFY_NEW) != 0);
    } else if (Handle{result.listener}.GetIndex() == static_cast<int>(g2)) {
      ++g2count;
      EXPECT_TRUE((result.flags & NT_NOTIFY_DELETE) != 0);
    } else if (Handle{result.listener}.GetIndex() == static_cast<int>(g3)) {
      ++g3count;
      EXPECT_TRUE((result.flags & NT_NOTIFY_UPDATE) != 0);
    } else if (Handle{result.listener}.GetIndex() == static_cast<int>(g4)) {
      ++g4count;
      EXPECT_TRUE((result.flags & NT_NOTIFY_FLAGS) != 0);
    } else {
      ADD_FAILURE() << "unknown listener index";
    }
  }
  EXPECT_EQ(g1count, 2);
  EXPECT_EQ(g2count, 1);  // NT_NOTIFY_DELETE
  EXPECT_EQ(g3count, 2);
  EXPECT_EQ(g4count, 2);
}

TEST_F(EntryNotifierTest, PollPrefixImmediate) {
  auto poller = notifier.CreatePoller();
  notifier.AddPolled(poller, "/foo", NT_NOTIFY_NEW | NT_NOTIFY_IMMEDIATE);
  notifier.AddPolled(poller, "/foo", NT_NOTIFY_NEW);

  ASSERT_FALSE(notifier.local_notifiers());

  GenerateNotifications();

  ASSERT_TRUE(notifier.WaitForQueue(1.0));
  bool timed_out = false;
  auto results = notifier.Poll(poller, 0, &timed_out);
  ASSERT_FALSE(timed_out);
  SCOPED_TRACE(::testing::PrintToString(results));
  ASSERT_EQ(results.size(), 4u);
}

TEST_F(EntryNotifierTest, PollPrefixLocal) {
  auto poller = notifier.CreatePoller();
  notifier.AddPolled(poller, "/foo", NT_NOTIFY_NEW | NT_NOTIFY_LOCAL);
  notifier.AddPolled(poller, "/foo", NT_NOTIFY_NEW);

  ASSERT_TRUE(notifier.local_notifiers());

  GenerateNotifications();

  ASSERT_TRUE(notifier.WaitForQueue(1.0));
  bool timed_out = false;
  auto results = notifier.Poll(poller, 0, &timed_out);
  ASSERT_FALSE(timed_out);
  SCOPED_TRACE(::testing::PrintToString(results));
  ASSERT_EQ(results.size(), 6u);
}

}  // namespace nt
