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
#ifndef LIT_PARTICLES_PARTICLE_UPDATER
#define LIT_PARTICLES_PARTICLE_UPDATER

#include "dynamics/dynamic.hh"
#include "projects/lit_particles/lit_particles_pool.hh"

namespace gem { namespace particle {
namespace lit_particles_project {
class LitParticleUpdater : public Dynamic<LitParticlesData> {
public:
  LitParticleUpdater() = default;
  ~LitParticleUpdater() = default;

  void Update(double a_dt, const std::shared_ptr<ParticlePool<LitParticlesData> >& a_pPool) override;
  bool AltersParticleLifeCycle() const override { return true; }
}; /* class LitParticleUpdater*/
} /* namespace lit_particles_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: LIT_PARTICLES_PARTICLE_UPDATER */
