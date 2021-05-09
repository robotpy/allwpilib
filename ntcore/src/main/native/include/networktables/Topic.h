// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <stdint.h>

#include <functional>
#include <utility>

#include <wpi/StringRef.h>

#include "networktables/NetworkTableType.h"
#include "ntcore_c.h"
#include "ntcore_cpp.h"

namespace wpi {
class json;
}  // namespace wpi

namespace nt {

class NetworkTableInstance;

class Topic {
 public:
  Topic() = default;
  explicit Topic(const wpi::Twine& name);
  explicit Topic(NT_Topic handle) : m_handle{handle} {}

  /**
   * Determines if the native handle is valid.
   *
   * @return True if the native handle is valid, false otherwise.
   */
  explicit operator bool() const { return m_handle != 0; }

  /**
   * Gets the native handle for the topic.
   *
   * @return Native handle
   */
  NT_Topic GetHandle() const;

  /**
   * Gets the instance for the topic.
   *
   * @return Instance
   */
  NetworkTableInstance GetInstance() const;

  /**
   * Gets the name of the topic.
   *
   * @return the topic's name
   */
  std::string GetName() const;

  /**
   * Gets the type of the topic.
   *
   * @return the topic's type
   */
  NetworkTableType GetType() const;

  /**
   * Gets the type string of the topic. This may have more information
   * than the numeric type (especially for raw values).
   *
   * @return the topic's type
   */
  std::string GetTypeString() const;

  /**
   * Make value persistent through program restarts.
   *
   * @param persistent True for persistent, false for not persistent.
   */
  void SetPersistent(bool persistent);

  /**
   * Returns whether the value is persistent through program restarts.
   *
   * @return True if the value is persistent.
   */
  bool IsPersistent() const;

  wpi::json GetProperty(const wpi::Twine& name) const;

  void SetProperty(const wpi::Twine& name, const wpi::json& value);

  void DeleteProperty(const wpi::Twine& name);

  wpi::json GetProperties() const;

  void SetProperties(const wpi::json& properties);

  void SetPropertiesListener(
      std::function<void(const wpi::json& properties)> listener);

  /**
   * Gets combined information about the topic.
   *
   * @return Topic information
   */
  TopicInfo GetInfo() const;

  /**
   * Equality operator.  Returns true if both instances refer to the same
   * native handle.
   */
  bool operator==(const Topic& oth) const { return m_handle == oth.m_handle; }

  /** Inequality operator. */
  bool operator!=(const Topic& oth) const { return !(*this == oth); }

 protected:
  NT_Topic m_handle{0};
};

class Subscriber {
 public:
  virtual ~Subscriber() = default;

  Subscriber(const Subscriber&) = delete;
  Subscriber& operator=(const Subscriber&) = delete;

  Subscriber(Subscriber&&);
  Subscriber& operator=(const Subscriber&&);

  /**
   * Gets the subscribed-to topic.
   *
   * @return Topic
   */
  Topic GetTopic() const;

  /**
   * Determines if the native handle is valid.
   *
   * @return True if the native handle is valid, false otherwise.
   */
  explicit operator bool() const { return m_subHandle != 0; }

  /**
   * Gets the native handle for the subscriber.
   *
   * @return Native handle
   */
  NT_Subscriber GetHandle() const { return m_subHandle; }

  /**
   * Determines if the topic is currently being published.
   *
   * @return True if the topic exists, false otherwise.
   */
  bool Exists() const;

  /**
   * Gets the last time the value was changed.
   * Note: this is not atomic with Get(); use GetAtomic() to get
   * both the value and last change as an atomic operation.
   *
   * @return Topic last change time
   */
  uint64_t GetLastChange() const;

  void SetDeletedListener(std::function<void()> listener);

 protected:
  Subscriber() = default;
  explicit Subscriber(NT_Subscriber handle);

  NT_Subscriber m_subHandle{0};
};

class Publisher {
 public:
  virtual ~Publisher() = default;

  Publisher(const Publisher&) = delete;
  Publisher& operator=(const Publisher&) = delete;

  Publisher(Publisher&&);
  Publisher& operator=(const Publisher&&);

 protected:
  Publisher() = default;
  explicit Publisher(NT_Publisher handle);

  NT_Publisher m_pubHandle{0};
};

class BooleanSubscriber : public Subscriber {
 public:
  BooleanSubscriber() = default;
  explicit BooleanSubscriber(Topic topic);

  bool Get() const;

  std::pair<bool, uint64_t> GetAtomic() const;

  void SetValueListener(std::function<void(bool, uint64_t)> listener);
};

class BooleanPublisher : public Publisher {
 public:
  BooleanPublisher() = default;
  explicit BooleanPublisher(Topic topic);

  void Set(bool value, uint64_t time = 0);

  void SetDefault(bool value);
};

class BooleanEntry : public BooleanSubscriber, public BooleanPublisher {
 public:
  BooleanEntry() = default;
  explicit BooleanEntry(NT_Topic topicHandle);
};

class DoubleSubscriber : public Subscriber {
 public:
  DoubleSubscriber() = default;;
  explicit DoubleSubscriber(Topic topic);

  double Get() const;

  std::pair<double, uint64_t> GetAtomic() const;

  void SetValueListener(std::function<void(double, uint64_t)> listener);
};

class DoublePublisher : public Publisher {
 public:
  DoublePublisher() = default;
  explicit DoublePublisher(Topic topic);

  void Set(double value, uint64_t time = 0);

  void SetDefault(double value);
};

class DoubleEntry : public DoubleSubscriber, public DoublePublisher {
 public:
  DoubleEntry() = default;
  explicit DoubleEntry(Topic topic);
};

class StringSubscriber : public Subscriber {
 public:
  StringSubscriber() = default;
  explicit StringSubscriber(Topic topic);

  std::string Get() const;
  wpi::StringRef Get(wpi::SmallVectorImpl<char>& buf);

  std::pair<std::string, uint64_t> GetAtomic() const;
  std::pair<wpi::StringRef, uint64_t> GetAtomic(
      wpi::SmallVectorImpl<char>& buf) const;

  void SetValueListener(std::function<void(wpi::StringRef, uint64_t)> listener);
};

class StringPublisher : public Publisher {
 public:
  StringPublisher() = default;
  explicit StringPublisher(Topic topic);

  void Set(wpi::StringRef value, uint64_t time = 0);

  void SetDefault(wpi::StringRef value);
};

class StringEntry : public StringSubscriber, public StringPublisher {
 public:
  StringEntry() = default;
  explicit StringEntry(Topic topic);
};

}  // namespace nt
