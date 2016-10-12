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
constexpr double SEC_PER_SEC    = 1.0; // duh...

// For some odd reasons, substraction of 2 std::chrono::time_point
// can only work with the type helper std::nanoseconds. But works
// flawlessly using the std::chrono::duration... So we define our
// own types here.
using nanoseconds   = std::chrono::duration<unsigned long long, std::nano>;
using microseconds  = std::chrono::duration<unsigned long long, std::micro>;
using milliseconds  = std::chrono::duration<unsigned long long, std::milli>;
using seconds       = std::chrono::duration<unsigned long long>;
using minutes       = std::chrono::duration<unsigned int, std::ratio<60> >;
using hours         = std::chrono::duration<unsigned int, std::ratio<3600> >;

template <typename DurationType>
unsigned long long GetTimeElapsed() {
  static std::chrono::time_point<std::chrono::system_clock> previous =
    std::chrono::system_clock::now();
  auto current = std::chrono::system_clock::now();
  DurationType elapsedTime(current-previous);
  previous = current;
  return elapsedTime.count();
}

inline double GetFPS() {
  return NANO_PER_SEC/GetTimeElapsed<nanoseconds>();
}

// Chrono version to prevent changing the current time at every
// calls to GetTimeElapsed.
namespace chrono {
namespace impl {
template <typename ClockType = std::chrono::system_clock>
struct Clock 
{
  static std::chrono::time_point<ClockType> current_time;
  static std::chrono::time_point<ClockType> previous_time;
};

template <typename ClockType>
std::chrono::time_point<ClockType> Clock<ClockType>::current_time = 
    ClockType::now();
template <typename ClockType>
std::chrono::time_point<ClockType> Clock<ClockType>::previous_time =
    ClockType::now();
} /* namespace impl */

template <typename DurationType>
inline unsigned long long GetTimeElapsed() {
  return DurationType(impl::Clock<>::current_time -
    impl::Clock<>::previous_time).count();
}

void Update() {
  impl::Clock<>::previous_time = impl::Clock<>::current_time;
  impl::Clock<>::current_time = std::chrono::system_clock::now();
}

inline double GetFPS() {
  return NANO_PER_SEC/GetTimeElapsed<nanoseconds>();
}
} /* namespace chrono */
} /* namespace timer */
#endif /* end of include guard: TIMER_HH_ */
