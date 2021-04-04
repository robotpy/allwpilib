// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.networktables;

/** NetworkTables double topic. */
public final class DoubleTopic extends Topic {
  /**
   * Construct from native handle.
   *
   * @param inst Instance
   * @param handle Native handle
   */
  public DoubleTopic(NetworkTableInstance inst, int handle) {
    super(inst, handle);
  }

  public DoubleSubscriber subscribe(double defaultValue, SubscribeOption... options) {
    return new DoubleEntryImpl(this, NetworkTablesJNI.subscribeDouble(m_handle, options), defaultValue);
  }

  public DoublePublisher publish(PublishOption... options) {
    return new DoubleEntryImpl(this, NetworkTablesJNI.publishDouble(m_handle, options), 0);
  }

  public DoubleEntry getEntry(double defaultValue, PubSubOption... options) {
    return new DoubleEntryImpl(this, NetworkTablesJNI.getEntryDouble(m_handle, options), defaultValue);
  }
}
