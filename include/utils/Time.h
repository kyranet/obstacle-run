// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <algorithm>
#include <chrono>

class Time {
  static int64_t elapsed_;
  static double_t delta_;

 public:
  [[nodiscard]] static inline std::chrono::time_point<std::chrono::steady_clock>
  now() noexcept {
    return std::chrono::steady_clock::now();
  }

  [[nodiscard]] static inline const double_t& delta() noexcept {
    return delta_;
  }

  [[nodiscard]] static inline const int64_t& elapsedNanoseconds() noexcept {
    return elapsed_;
  }

  [[nodiscard]] static inline int64_t elapsedMicroseconds() noexcept {
    return static_cast<int64_t>(static_cast<float_t>(elapsed_) / 1000.0f);
  }

  [[nodiscard]] static inline int64_t elapsedMilliSeconds() noexcept {
    return static_cast<int64_t>(static_cast<float_t>(elapsed_) / 1000000.0f);
  }

  static inline void measure(
      const std::chrono::time_point<std::chrono::steady_clock>& a,
      const double_t& frameTime) noexcept {
    const auto difference = now() - a;
    const auto nanoseconds = difference.count();
    elapsed_ = nanoseconds;

    const auto delta = static_cast<double_t>(elapsed_) / 1000000.0;
    delta_ = std::max(delta, frameTime) / 1000.0;
  }
};