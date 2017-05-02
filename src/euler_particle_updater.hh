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
#ifndef EULER_PARTICLE_UPDATER
#define EULER_PARTICLE_UPDATER

#include "dynamic.hh"
#include "particle_pool_core.hh"

namespace gem {
namespace particle {
class EulerParticleUpdater : public Dynamic<CoreParticles> {
public:
  EulerParticleUpdater() = default;
  ~EulerParticleUpdater() = default;

  void Update(double a_dt, const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool) override;
}; /* class EulerParticleUpdater*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: EULER_PARTICLE_UPDATER */
