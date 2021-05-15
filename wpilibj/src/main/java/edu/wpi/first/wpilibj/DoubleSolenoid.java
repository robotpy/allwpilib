// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj;

import edu.wpi.first.hal.FRCNetComm.tResourceType;
import edu.wpi.first.hal.HAL;
import edu.wpi.first.hal.util.UncleanStatusException;
import edu.wpi.first.wpilibj.smartdashboard.SendableBuilder;
import edu.wpi.first.wpilibj.smartdashboard.SendableRegistry;

import java.util.Objects;

/**
 * DoubleSolenoid class for running 2 channels of high voltage Digital Output on the PCM.
 *
 * <p>The DoubleSolenoid class is typically used for pneumatics solenoids that have two positions
 * controlled by two separate channels.
 */
public class DoubleSolenoid implements Sendable, AutoCloseable {
  /** Possible values for a DoubleSolenoid. */
  public enum Value {
    kOff,
    kForward,
    kReverse
  }

  private int m_forwardMask; // The mask for the forward channel.
  private int m_reverseMask; // The mask for the reverse channel.
  private final int m_mask; // The channel mask
  private PneumaticsBase m_module;
  private final int m_forwardChannel;
  private final int m_reverseChannel;

  /**
   * Constructor.
   *
   * @param moduleNumber The module number of the solenoid module to use.
   * @param forwardChannel The forward channel on the module to control (0..7).
   * @param reverseChannel The reverse channel on the module to control (0..7).
   */
  public DoubleSolenoid(
      PneumaticsBase module, final int forwardChannel, final int reverseChannel) {

    m_module = Objects.requireNonNull(module, "Module cannot be null");

    m_forwardChannel = forwardChannel;
    m_reverseChannel = reverseChannel;

    m_forwardMask = (1 << forwardChannel);
    m_reverseMask = (1 << reverseChannel);
    m_mask = m_forwardMask | m_reverseMask;

    HAL.report(tResourceType.kResourceType_Solenoid, forwardChannel + 1, module.getModuleNumber() + 1);
    HAL.report(tResourceType.kResourceType_Solenoid, reverseChannel + 1, module.getModuleNumber() + 1);
    SendableRegistry.addLW(this, "DoubleSolenoid", module.getModuleNumber(), forwardChannel);
  }

  public PneumaticsBase getModule() {
    return m_module;
  }

  @Override
  public synchronized void close() {
    SendableRegistry.remove(this);
    m_module = null;
  }

  /**
   * Set the value of a solenoid.
   *
   * @param value The value to set (Off, Forward, Reverse)
   */
  public void set(final Value value) {
    int setValue;

    switch (value) {
      case kOff:
        setValue = 0;
        break;
      case kForward:
        setValue = m_forwardMask;
        break;
      case kReverse:
        setValue = m_reverseMask;
        break;
      default:
        throw new AssertionError("Illegal value: " + value);
    }

    m_module.setSolenoids(m_mask, setValue);
  }

  /**
   * Read the current value of the solenoid.
   *
   * @return The current value of the solenoid.
   */
  public Value get() {
    int values = m_module.getSolenoids();

    if ((values & m_forwardMask) != 0) {
      return Value.kForward;
    } else if ((values & m_reverseMask) != 0) {
      return Value.kReverse;
    } else {
      return Value.kOff;
    }
  }

  /**
   * Toggle the value of the solenoid.
   *
   * <p>If the solenoid is set to forward, it'll be set to reverse. If the solenoid is set to
   * reverse, it'll be set to forward. If the solenoid is set to off, nothing happens.
   */
  public void toggle() {
    Value value = get();

    if (value == Value.kForward) {
      set(Value.kReverse);
    } else if (value == Value.kReverse) {
      set(Value.kForward);
    }
  }

  /**
   * Get the forward channel.
   *
   * @return the forward channel.
   */
  public int getFwdChannel() {
    return m_forwardChannel;
  }

  /**
   * Get the reverse channel.
   *
   * @return the reverse channel.
   */
  public int getRevChannel() {
    return m_reverseChannel;
  }

  /**
   * Check if the forward solenoid is blacklisted. If a solenoid is shorted, it is added to the
   * blacklist and disabled until power cycle, or until faults are cleared.
   *
   * @return If solenoid is disabled due to short.
   * @see #clearAllPCMStickyFaults()
   */
  public boolean isFwdSolenoidBlackListed() {
    // TODO
    // int blackList = getPCMSolenoidBlackList();
    // return (blackList & m_forwardMask) != 0;
    return false;
  }

  /**
   * Check if the reverse solenoid is blacklisted. If a solenoid is shorted, it is added to the
   * blacklist and disabled until power cycle, or until faults are cleared.
   *
   * @return If solenoid is disabled due to short.
   * @see #clearAllPCMStickyFaults()
   */
  public boolean isRevSolenoidBlackListed() {
    // int blackList = getPCMSolenoidBlackList();
    // return (blackList & m_reverseMask) != 0;
    return false;
  }

  @Override
  public void initSendable(SendableBuilder builder) {
    builder.setSmartDashboardType("Double Solenoid");
    builder.setActuator(true);
    builder.setSafeState(() -> set(Value.kOff));
    builder.addStringProperty(
        "Value",
        () -> get().name().substring(1),
        value -> {
          if ("Forward".equals(value)) {
            set(Value.kForward);
          } else if ("Reverse".equals(value)) {
            set(Value.kReverse);
          } else {
            set(Value.kOff);
          }
        });
  }
}
