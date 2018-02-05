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
#include "utils/singleton.hh"

namespace timer {
// Utility definitions, since there's an issue
// with the one defined by the STL with the calls
// made in this file
constexpr double NANO_PER_SEC   = 1.0e09;
constexpr double MICRO_PER_SEC  = 1.0e06;
constexpr double MILLI_PER_SEC  = 1.0e03;

// Standalone version, without the chrono. 
// The "state" is contained in the static
// local variable.
template <typename DurationType>
inline unsigned long long UpdateAndGetTimeElapsed() {
  static std::chrono::time_point<std::chrono::steady_clock> previous =
    std::chrono::steady_clock::now();
  auto current = std::chrono::steady_clock::now();
  DurationType elapsedTime = std::chrono::duration_cast<DurationType>(current-previous);
  previous = current;
  return elapsedTime.count();
}
inline double UpdateAndGetFPS() {
  return NANO_PER_SEC/UpdateAndGetTimeElapsed<std::chrono::nanoseconds>();
}

// Chrono version to prevent changing the current time at every
// calls to GetTimeElapsed.
class Chrono : public Singleton<Chrono> {
public:
  Chrono() : _CurrentTime(std::chrono::steady_clock::now()),
    _PreviousTime(std::chrono::steady_clock::now()) {}

  ~Chrono() = default;

  template <typename DurationType>
  inline unsigned long long GetTimeElapsed() const {
    return std::chrono::duration_cast<DurationType>(
      abs(_CurrentTime -_PreviousTime)).count();
  }

  double GetTimeElapsedInSeconds() const {
    return GetTimeElapsed<std::chrono::nanoseconds>() / NANO_PER_SEC;
  }

  inline void Update() {
    _PreviousTime = _CurrentTime;
    _CurrentTime = std::chrono::steady_clock::now();
  }

  inline double GetFPS() const {
    return NANO_PER_SEC / GetTimeElapsed<std::chrono::nanoseconds>();
  }

private:
  std::chrono::steady_clock::time_point _CurrentTime;
  std::chrono::steady_clock::time_point _PreviousTime;
}; /* class Chrono */
} /* namespace timer */

#endif /* end of include guard: TIMER_HH_ */
