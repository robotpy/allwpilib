// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.networktables;

/** NetworkTables double subscriber. */
abstract class EntryImpl implements Subscriber, Publisher {
  /**
   * Constructor.
   *
   * @param handle handle
   */
  public EntryImpl(int handle) {
  }

  @Override
  public void close() {

  }

  @Override
  public boolean exists() {
    return NetworkTablesJNI.getType(m_handle) != 0;
  }

  @Override
  public long getLastChange() {
    return NetworkTablesJNI.getEntryLastChange(m_handle);
  }

  protected int m_handle;
}
