/*************************************************************************
 * Copyright (c) 2016 François Trudel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
*************************************************************************/
#ifndef TIMER_HH_
#define TIMER_HH_

#include <chrono>

namespace timer {
constexpr double NANO_PER_SEC   = 1.0e09;
constexpr double MICRO_PER_SEC  = 1.0e06;
constexpr double MILLI_PER_SEC  = 1.0e03;

template <typename DurationType>
unsigned long long GetTimeElapsed() {
  static std::chrono::time_point<std::chrono::steady_clock> previous =
    std::chrono::steady_clock::now();
  auto current = std::chrono::steady_clock::now();
  DurationType elapsedTime = std::chrono::duration_cast<DurationType>(current-previous);
  previous = current;
  return elapsedTime.count();
}

inline double GetFPS() {
  return NANO_PER_SEC/GetTimeElapsed<std::chrono::nanoseconds>();
}

// Chrono version to prevent changing the current time at every
// calls to GetTimeElapsed.
namespace chrono {
namespace impl {
struct Clock 
{
  static std::chrono::steady_clock::time_point current_time;
  static std::chrono::steady_clock::time_point previous_time;
};

std::chrono::steady_clock::time_point Clock::current_time =
    std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point Clock::previous_time =
    std::chrono::steady_clock::now();
} /* namespace impl */

template <typename DurationType>
inline unsigned long long GetTimeElapsed() {
  return std::chrono::duration_cast<DurationType>(
    impl::Clock::current_time -
    impl::Clock::previous_time).count();
}

void Update() {
  impl::Clock::previous_time = impl::Clock::current_time;
  impl::Clock::current_time = std::chrono::steady_clock::now();
}

inline double GetFPS() {
  return NANO_PER_SEC/GetTimeElapsed<std::chrono::nanoseconds>();
}
} /* namespace chrono */
} /* namespace timer */
#endif /* end of include guard: TIMER_HH_ */
