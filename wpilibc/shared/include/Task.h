/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008-2016. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <iostream>
#include <string>
#include <thread>

#include "ErrorBase.h"
#include "HAL/Task.h"

/**
 * Wrapper class around std::thread that allows changing thread priority
 */
class Task : public ErrorBase {
 public:
  static const int kDefaultPriority = 60;

  Task() = default;
  Task(const Task&) = delete;
  Task& operator=(const Task&) = delete;
  Task& operator=(Task&& task);

  template <class Function, class... Args>
  Task(const std::string& name, Function&& function, Args&&... args);

  virtual ~Task();

  bool joinable() const noexcept;
  void join();
  void detach();
  std::thread::id get_id() const noexcept;
  std::thread::native_handle_type native_handle();

  bool Verify();

  int GetPriority();

  bool SetPriority(int priority);

  std::string GetName() const;

 private:
  std::thread m_thread;
  std::string m_taskName;
  bool HandleError(int32_t results);
};

#include "Task.inc"
