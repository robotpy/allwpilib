/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_IMAGESINKIMPL_H_
#define CSCORE_IMAGESINKIMPL_H_

#include <stdint.h>

#include <atomic>
#include <memory>

#include <opencv2/core/core.hpp>
#include <wpi/Signal.h>
#include <wpi/Twine.h>
#include <wpi/condition_variable.h>

#include "Frame.h"
#include "SinkImpl.h"
#include "cscore_raw.h"

namespace cs {

class SourceImpl;

class ImageSinkImpl : public SinkImpl {
 public:
  ImageSinkImpl(const wpi::Twine& name, wpi::Logger& logger);
  ~ImageSinkImpl() override;

  void Stop();

  uint64_t GrabFrame(cv::Mat& image);
  uint64_t GrabFrame(cv::Mat& image, double timeout);

  uint64_t GrabFrame(CS_RawFrame& frame);
  uint64_t GrabFrame(CS_RawFrame& frame, double timeout);

 private:
  Frame GetNextFrame(bool hasTimeout, double timeout);
  uint64_t GrabFrameImpl(cv::Mat& image, Frame&& frame);
  uint64_t GrabFrameImpl(CS_RawFrame& rawFrame, Frame&& frame);

  void SetSourceImpl(std::shared_ptr<SourceImpl> source) override;

  std::atomic_bool m_active{true};
  wpi::condition_variable m_frameCv;
  Frame m_frame;
  uint64_t m_lastFrameTime{0};
  wpi::sig::ScopedConnection m_onNewFrame;
};

}  // namespace cs

#endif  // CSCORE_IMAGESINKIMPL_H_
