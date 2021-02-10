// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <memory>

#include <hal/Types.h>
#include <units/time.h>

#include "frc/ErrorBase.h"

namespace frc {
class DigitalSource;

/**
 * Class for handling ssynchronous interrupts.
 *
 * <p> By default, interrupts will occur on rising edge.
 *
 * <p> Asynchronous interrupts are handled by the AsynchronousInterrupt class.
 */
class SynchronousInterrupt : public ErrorBase {
 public:
  enum WaitResult {
    kTimeout = 0x0,
    kRisingEdge = 0x1,
    kFallingEdge = 0x100,
    kBoth = 0x101,
  };

  /**
   * Construct a Synchronous Interrupt from a Digital Source.
   */
  explicit SynchronousInterrupt(DigitalSource& source);

  /**
   * Construct a Synchronous Interrupt from a Digital Source.
   */
  explicit SynchronousInterrupt(DigitalSource* source);

  /**
   * Construct a Synchronous Interrupt from a Digital Source.
   */
  explicit SynchronousInterrupt(std::shared_ptr<DigitalSource> source);

  ~SynchronousInterrupt() override;

  SynchronousInterrupt(SynchronousInterrupt&&) = default;
  SynchronousInterrupt& operator=(SynchronousInterrupt&&) = default;

  /**
   * Wait for an interrupt to occur.
   *
   * <p> Both rising and falling edge can be returned if both a rising and
   * falling happened between calls, and ignorePrevious is false.
   *
   * @param timeout The timeout to wait for. 0s or less will return immediately.
   * @param ignorePrevious True to ignore any previous interrupts, false to
   * return interrupt value if an interrupt has occured since last call.
   * @return The edge(s) that were triggered, or timeout.
   */
  WaitResult WaitForInterrupt(units::second_t timeout,
                              bool ignorePrevious = false);

  /**
   * Set which edges cause an interrupt to occur.
   *
   * @param risingEdge true to trigger on rising edge, false otherwise.
   * @param fallingEdge true to trigger on falling edge, false otherwise
   */
  void SetInterruptEdges(bool risingEdge, bool fallingEdge);

  /**
   * Get the timestamp (relative to FPGA Time) of the last rising edge.
   */
  units::second_t GetRisingTimestamp();

  /**
   * Get the timestamp of the last falling edge.
   *
   * <p>This function does not require the interrupt to be enabled to work.
   *
   * <p>This only works if falling edge was configured using setInterruptEdges.
   * @return the timestamp in seconds relative to getFPGATime
   */
  units::second_t GetFallingTimestamp();

  /**
   * Wake up an existing wait call. Can be called from any thread.
   */
  void WakeupWaitingInterrupt();

 private:
  void InitSynchronousInterrupt();
  std::shared_ptr<DigitalSource> m_source;
  hal::Handle<HAL_InterruptHandle> m_handle;
};
}  // namespace frc
