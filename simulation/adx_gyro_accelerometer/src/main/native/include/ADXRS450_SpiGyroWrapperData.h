/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <atomic>
#include <memory>

#include <support/mutex.h>

#include "MockData/NotifyListenerVector.h"

namespace hal {
class ADXRS450_SpiGyroWrapper {
 public:
  explicit ADXRS450_SpiGyroWrapper(int port);
  virtual ~ADXRS450_SpiGyroWrapper();

  void HandleRead(uint8_t* buffer, uint32_t count);
  void HandleAutoReceiveData(uint8_t* buffer, int32_t numToRead,
                             int32_t& outputCount);

  virtual void ResetData();

  int32_t RegisterAngleCallback(HAL_NotifyCallback callback, void* param,
                                HAL_Bool initialNotify);
  void CancelAngleCallback(int32_t uid);
  void InvokeAngleCallback(HAL_Value value);
  double GetAngle();
  void SetAngle(double angle);

 private:
  int m_port;
  int m_readCallbackId;
  int m_autoReceiveReadCallbackId;

  wpi::mutex m_registerMutex;
  std::atomic<double> m_angle{0.0};
  std::atomic<double> m_angle_diff{0.0};
  std::shared_ptr<NotifyListenerVector> m_angleCallbacks = nullptr;

  static const double ANGLE_LSB;
  static const double MAX_ANGLE_DELTA_PER_MESSAGE;
  static const int PACKET_SIZE;
};
}  // namespace hal
