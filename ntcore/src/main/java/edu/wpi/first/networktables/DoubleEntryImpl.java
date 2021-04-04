// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.networktables;

/** NetworkTables double subscriber. */
final class DoubleEntryImpl extends EntryImpl implements DoubleEntry {
  /**
   * Constructor.
   *
   * @param topic Topic
   * @param handle Native handle
   * @param defaultValue Default value for get()
   */
  public DoubleEntryImpl(DoubleTopic topic, int handle, double defaultValue) {
    super(handle);
    m_topic = topic;
    m_defaultValue = defaultValue;
  }

  @Override
  public DoubleTopic getTopic() {
    return m_topic;
  }

  @Override
  public double get() {
    return NetworkTablesJNI.getDouble(m_handle, m_defaultValue);
  }

  @Override
  public double get(double defaultValue) {
    return NetworkTablesJNI.getDouble(m_handle, defaultValue);
  }

  @Override
  public TimestampedDouble getAtomic() {
    return NetworkTablesJNI.getAtomicDouble(m_handle, m_defaultValue);
  }

  @Override
  public TimestampedDouble getAtomic(double defaultValue) {
    return NetworkTablesJNI.getAtomicDouble(m_handle, defaultValue);
  }

  @Override
  public void setValueListener(ValueListener listener) {
    // TODO
  }

  @Override
  public void set(double value, long time) {
    NetworkTablesJNI.setDouble(m_handle, time, value, false);
  }

  @Override
  public void setDefault(double value, long time) {
    NetworkTablesJNI.setDefaultDouble(m_handle, time, value);
  }

  private final DoubleTopic m_topic;
  private final double m_defaultValue;
}
