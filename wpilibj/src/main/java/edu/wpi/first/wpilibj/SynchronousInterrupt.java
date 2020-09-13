/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj;

import edu.wpi.first.hal.InterruptJNI;

import static edu.wpi.first.wpilibj.util.ErrorMessages.requireNonNullParam;

/**
 * Class for handling sychronous interrupts.
 *
 * <p>Asynchronous interrupts are handled by the AsynchronousInterrupt class.
 */
public class SynchronousInterrupt implements AutoCloseable {
  @SuppressWarnings("PMD.SingularField")
  private final DigitalSource m_source;
  private final int m_handle;

  @SuppressWarnings("JavadocMethod")
  public enum WaitResult {
    kTimeout(0x0), kRisingEdge(0x1), kFallingEdge(0x100), kBoth(0x101);

    @SuppressWarnings("MemberName")
    public final int value;

    WaitResult(int value) {
      this.value = value;
    }

    public static WaitResult getValue(boolean rising, boolean falling) {
      if (rising && falling) {
        return kBoth;
      } else if (rising) {
        return kRisingEdge;
      } else if (falling) {
        return kFallingEdge;
      } else {
        return kTimeout;
      }
    }
  }

  /**
   * Constructs a new synchronous interrupt using a DigitalSource.
   *
   * <p>At construction, the interrupt will trigger on the rising edge.
   *
   * @param source The digital source to use.
   */
  public SynchronousInterrupt(DigitalSource source) {
    m_source = requireNonNullParam(source, "source", "SynchronousInterrupt");
    m_handle = InterruptJNI.initializeInterrupts(true);
    InterruptJNI.requestInterrupts(m_handle, m_source.getPortHandleForRouting(),
        m_source.getAnalogTriggerTypeForRouting());
    InterruptJNI.setInterruptUpSourceEdge(m_handle, true, false);
  }

  /**
   * Closes the interrupt.
   *
   * <p>This does not close the associated digital source.
   */
  @Override
  public void close() throws Exception {
    InterruptJNI.cleanInterrupts(m_handle);
  }

  /**
   * Wait for interrupt that returns the raw result value from the hardware.
   *
   * <p>Used by AsynchronousInterrupt. Users prefer to use waitForInterrupt.
   *
   * @param timeoutSeconds The timeout in seconds. 0 or less will return immediately.
   * @param ignorePrevious True to ignore if a previous interrupt has occured,
   *     and only wait for a new trigger. False will consider if an interrupt has occured
   *     since the last time the interrupt was read.
   * @return The raw hardware interrupt result
   */
  int waitForInterruptRaw(double timeoutSeconds, boolean ignorePrevious) {
    return InterruptJNI.waitForInterrupt(m_handle, timeoutSeconds, ignorePrevious);
  }

  /**
   * Wait for an interrupt.
   *
   * @param timeoutSeconds The timeout in seconds. 0 or less will return immediately.
   * @param ignorePrevious True to ignore if a previous interrupt has occured,
   *     and only wait for a new trigger. False will consider if an interrupt has occured
   *     since the last time the interrupt was read.
   * @return Result of which edges were triggered, or if an timeout occured.
   */
  public WaitResult waitForInterrupt(double timeoutSeconds, boolean ignorePrevious) {
    int result = InterruptJNI.waitForInterrupt(m_handle, timeoutSeconds, ignorePrevious);

    // Rising edge result is the interrupt bit set in the byte 0xFF
    // Falling edge result is the interrupt bit set in the byte 0xFF00
    // Set any bit set to be true for that edge, and AND the 2 results
    // together to match the existing enum for all interrupts
    boolean rising = (result & 0xFF) != 0;
    boolean falling = (result & 0xFF00) != 0;
    return WaitResult.getValue(rising, falling);
  }

  /**
   * Set which edges to trigger the interrupt on.
   *
   * @param risingEdge Trigger on rising edge
   * @param fallingEdge Trigger on falling edge
   */
  public void setInterruptEdges(boolean risingEdge, boolean fallingEdge) {
    InterruptJNI.setInterruptUpSourceEdge(m_handle, risingEdge, fallingEdge);
  }

  /**
   * Get the timestamp of the last rising edge.
   *
   * <p>This only works if rising edge was configured using setInterruptEdges.
   * @return the timestamp in seconds relative to getFPGATime
   */
  public double getRisingTimestamp() {
    return InterruptJNI.readInterruptRisingTimestamp(m_handle) * 1e-6;
  }

  /**
   * Get the timestamp of the last falling edge.
   *
   * <p>This only works if falling edge was configured using setInterruptEdges.
   * @return the timestamp in seconds relative to getFPGATime
   */
  public double getFallingTimestamp() {
    return InterruptJNI.readInterruptFallingTimestamp(m_handle) * 1e-6;
  }

  /**
   * Force triggering of any waiting interrupt, which will be seen as a timeout.
   */
  public void wakeupWaitingInterrupt() {
    InterruptJNI.releaseWaitingInterrupt(m_handle);
  }
}