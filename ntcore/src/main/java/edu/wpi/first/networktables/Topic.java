// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.networktables;

import java.util.function.Consumer;

/** NetworkTables Topic. */
public class Topic {
  /**
   * Construct from native handle.
   *
   * @param inst Instance
   * @param handle Native handle
   */
  public Topic(NetworkTableInstance inst, int handle) {
    m_inst = inst;
    m_handle = handle;
  }

  /**
   * Determines if the native handle is valid.
   *
   * @return True if the native handle is valid, false otherwise.
   */
  public boolean isValid() {
    return m_handle != 0;
  }

  /**
   * Gets the native handle for the topic.
   *
   * @return Native handle
   */
  public int getHandle() {
    return m_handle;
  }

  /**
   * Gets the instance for the topic.
   *
   * @return Instance
   */
  public NetworkTableInstance getInstance() {
    return m_inst;
  }

  /**
   * Gets the name of the topic.
   *
   * @return the topic's name
   */
  public String getName() {
    return NetworkTablesJNI.getTopicName(m_handle);
  }

  /**
   * Gets the type of the topic.
   *
   * @return the topic's type
   */
  public NetworkTableType getType() {
    return NetworkTableType.getFromInt(NetworkTablesJNI.getType(m_handle));
  }

  /**
   * Gets the type string of the topic. This may have more information
   * than the numeric type (especially for raw values).
   *
   * @return the topic's type
   */
  String getTypeString() {
    return NetworkTablesJNI.getTypeString(m_handle);
  }

  /**
   * Gets combined information about the topic.
   *
   * @return Topic information
   */
  public TopicInfo getInfo() {
    return NetworkTablesJNI.getTopicInfoHandle(m_inst, m_handle);
  }

  /** Make value persistent through program restarts.
   *
   * @param persistent True for persistent, false for not persistent.
   */
  public void setPersistent(boolean persistent) {
    NetworkTablesJNI.setTopicPersistent(m_handle, persistent);
  }

  /**
   * Returns whether the value is persistent through program restarts.
   *
   * @return True if the value is persistent.
   */
  public boolean isPersistent() {
    return NetworkTablesJNI.getTopicPersistent(m_handle);
  }

  /**
   * Add a listener for changes to the entry.
   *
   * @param listener the listener to add
   * @param flags bitmask specifying desired notifications
   * @return listener handle
   */
   /*
  public int setPropertiesListener(Consumer<EntryNotification> listener, int flags) {
    return m_inst.addEntryListener(this, listener, flags);
  }
  */

  @Override
  public boolean equals(Object other) {
    if (other == this) {
      return true;
    }
    if (!(other instanceof Topic)) {
      return false;
    }

    return m_handle == ((Topic) other).m_handle;
  }

  @Override
  public int hashCode() {
    return m_handle;
  }

  protected NetworkTableInstance m_inst;
  protected int m_handle;
}
