/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_CVSINKIMPL_H_
#define CSCORE_CVSINKIMPL_H_

#include <stdint.h>

#include <opencv2/core/core.hpp>
#include <wpi/Twine.h>

#include "Frame.h"
#include "SinkImpl.h"

namespace cs {

class SourceImpl;

class CvSinkImpl : public SinkImpl {
 public:
  CvSinkImpl(const wpi::Twine& name, wpi::Logger& logger, Notifier& notifier);
  ~CvSinkImpl() override;

  void Stop();

  uint64_t GrabFrame(cv::Mat& image);
  uint64_t GrabFrame(cv::Mat& image, double timeout);
};

}  // namespace cs

#endif  // CSCORE_CVSINKIMPL_H_
