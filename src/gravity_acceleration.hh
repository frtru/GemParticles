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
#ifndef GLOBAL_ACCELERATION_HH
#define GLOBAL_ACCELERATION_HH

#include "dynamic.hh"

namespace gem {
namespace particle {
class GravityAcceleration : public Dynamic {
private:
  static const glm::f32vec3 GRAVITY_ACCEL;
public:
  GravityAcceleration() = default;
  ~GravityAcceleration() = default;

	// TODO: Copyable and moveable?<

  virtual void Update(double a_dt, const std::shared_ptr<ParticlePoolCore>& a_pPool) override;
}; /* class EulerParticleUpdater*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: GLOBAL_ACCELERATION_HH */
