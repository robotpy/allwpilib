/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <array>
#include <functional>
#include <map>
#include <utility>
#include <vector>

#include <wpi/MathExtras.h>

#include "frc/geometry/Pose2d.h"
#include "units/time.h"

namespace frc {

/**
 * The TimeInterpolatableBuffer provides an easy way to estimate past
 * measurements. One application might be in conjunction with the
 * DifferentialDrivePoseEstimator, where knowledge of the robot pose at the time
 * when vision or other global measurement were recorded is necessary, or for
 * recording the past angles of mechanisms as measured by encoders.
 *
 * @param <T> The type stored in this buffer.
 */
template <typename T>
class TimeInterpolatableBuffer {
 public:
  /**
   * Create a new TimeInterpolatableBuffer.
   *
   * @param historySizeSeconds  The history size of the buffer.
   * @param interpolateFunction The function used to interpolate between values.
   */
  TimeInterpolatableBuffer(units::second_t historySize,
                           std::function<T(const T&, const T&, double)> func)
      : m_historySize(historySize), m_interpolatingFunc(func) {}

  /**
   * Create a new TimeInterpolatableBuffer. By default, the interpolation
   * function is wpi::Lerp except for Pose2d, which uses pose exponentials.
   *
   * @param historySizeSeconds  The history size of the buffer.
   * @param interpolateFunction The function used to interpolate between values.
   */
  explicit TimeInterpolatableBuffer(units::second_t historySize)
      : m_historySize(historySize),
        m_interpolatingFunc([](const T& start, const T& end, double t) {
          return wpi::Lerp(start, end, t);
        }) {}

  /**
   * Add a sample to the buffer.
   *
   * @param time   The timestamp of the sample.
   * @param sample The sample object.
   */
  void AddSample(units::second_t time, T sample) {
    // Add the new state into the vector.
    if (m_pastSnapshots.size() == 0 || time > m_pastSnapshots.back().first)
      m_pastSnapshots.emplace_back(time, sample);
    else
      m_pastSnapshots.insert(
          std::upper_bound(
              m_pastSnapshots.begin(), m_pastSnapshots.end(), time,
              [](auto t, const auto& pair) { return t < pair.first; }),
          std::pair(time, sample));
    while (time - m_pastSnapshots[0].first > m_historySize)
      m_pastSnapshots.erase(m_pastSnapshots.begin());
  }

  void Clear() { m_pastSnapshots.clear(); }

  /**
   * Sample the buffer at the given time. If there are no elements in the
   * buffer, calling this function results in undefined behavior.
   */
  T Sample(units::second_t time) {
    // We will perform a binary search to find the index of the element in the
    // vector that has a timestamp that is equal to or greater than the vision
    // measurement timestamp.

    if (time <= m_pastSnapshots.front().first)
      return m_pastSnapshots.front().second;
    if (time > m_pastSnapshots.back().first)
      return m_pastSnapshots.back().second;
    if (m_pastSnapshots.size() < 2) return m_pastSnapshots[0].second;

    // Get the iterator which has a key no less than the requested key.
    auto upper_bound = std::lower_bound(
        m_pastSnapshots.begin(), m_pastSnapshots.end(), time,
        [](const auto& pair, auto t) { return t > pair.first; });

    auto lower_bound = upper_bound - 1;

    double t = ((time - lower_bound->first) /
                (upper_bound->first - lower_bound->first));

    return m_interpolatingFunc(lower_bound->second, upper_bound->second, t);
  }

 private:
  units::second_t m_historySize;
  std::vector<std::pair<units::second_t, T>> m_pastSnapshots;
  std::function<T(const T&, const T&, double)> m_interpolatingFunc;
};

// Template specialization to ensure that Pose2d uses pose exponential
template <>
TimeInterpolatableBuffer<Pose2d>::TimeInterpolatableBuffer(
    units::second_t historySize)
    : m_historySize(historySize),
      m_interpolatingFunc([](const Pose2d& start, const Pose2d& end, double t) {
        if (t < 0) {
          return start;
        } else if (t >= 1) {
          return end;
        } else {
          Twist2d twist = start.Log(end);
          Twist2d scaledTwist = twist * t;
          return start.Exp(scaledTwist);
        }
      }) {}

}  // namespace frc
