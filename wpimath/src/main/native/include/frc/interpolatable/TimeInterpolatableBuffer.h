/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <array>
#include <utility>
#include <vector>
#include <functional>

#include <wpi/MathExtras.h>

#include "units/time.h"

namespace frc {

template <typename T>
class TimeInterpolatableBuffer {
 public:
  explicit TimeInterpolatableBuffer(std::function<T(const T&, const T&, double)> func
    = [](const T& start, const T& end, double t) { return wpi::Lerp(start, end, t); }) :
    m_interpolatingFunc(func) {}

  void AddSample(units::second_t time, T sample) {
    // Add the new state into the vector.
    m_pastSnapshots.emplace_back(time, sample);

    // Remove the oldest snapshot if the vector exceeds our maximum size.
    while (!m_pastSnapshots.empty()) {
      const auto& entry = m_pastSnapshots.back();
      if (time - entry.first >= m_historySize) {
        m_pastSnapshots.pop_back();
      } else {
        return;
      }
    }
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

    if (m_pastSnapshots.size() < 2) return m_pastSnapshots[0].second;

    int low = 0;
    int high = m_pastSnapshots.size() - 1;

    while (low != high) {
      int mid = (low + high) / 2.0;
      if (m_pastSnapshots[mid].first < time) {
        // This index and everything under it are less than the requested
        // time. Therefore, we can discard them.
        low = mid + 1;
      } else {
        // t is at least as large as the element at this index. This means that
        // anything after it cannot be what we are looking for.
        high = mid;
      }
    }

    // Because low and high are now the same (both "high"), we decrement low
    low -= 1;

    const auto& bottomBound = m_pastSnapshots[low];
    const auto& topBound = m_pastSnapshots[high];

    return wpi::Lerp(
        bottomBound.second, topBound.second,
        (time - bottomBound.first) / (topBound.first - bottomBound.first));
  }

 private:
  static constexpr units::second_t m_historySize = 10_s;
  std::vector<std::pair<units::second_t, T>> m_pastSnapshots;
  std::function<T(const T&, const T&, double)> m_interpolatingFunc;
};

}  // namespace frc
