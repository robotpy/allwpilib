// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.networktables;

/** NetworkTables topic information. */
public final class TopicInfo {
  /** Topic handle. */
  @SuppressWarnings("MemberName")
  public final int topic;

  /** Topic name. */
  @SuppressWarnings("MemberName")
  public final String name;

  /** Topic type. */
  @SuppressWarnings("MemberName")
  public final NetworkTableType type;

  /** Topic type string. */
  @SuppressWarnings("MemberName")
  public final String typeStr;

  /** Timestamp of last change to topic value. */
  @SuppressWarnings("MemberName")
  public final long lastChange;

  /**
   * Constructor. This should generally only be used internally to NetworkTables.
   *
   * @param inst Instance
   * @param handle Topic handle
   * @param name Name
   * @param type Type (integer version of {@link NetworkTableType})
   * @param typeStr Type string
   * @param lastChange Timestamp of last change
   */
  public TopicInfo(
      NetworkTableInstance inst, int handle, String name, int type, String typeStr, long lastChange) {
    this.m_inst = inst;
    this.topic = handle;
    this.name = name;
    this.type = NetworkTableType.getFromInt(type);
    this.typeStr = typeStr;
    this.lastChange = lastChange;
  }

  /* Network table instance. */
  private final NetworkTableInstance m_inst;

  /* Cached topic object. */
  private Topic m_topicObject;

  /**
   * Get the topic as an object.
   *
   * @return Topic
   */
  Topic getTopic() {
    if (m_topicObject == null) {
      m_topicObject = new Topic(m_inst, topic);
    }
    return m_topicObject;
  }
}
