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
 *
*************************************************************************/
#ifndef LIT_PARTICLES_SYSTEM_HH
#define LIT_PARTICLES_SYSTEM_HH

#include "particle_systems/particle_system.hh"
#include "utils/light_module.hh"
#include "projects/lit_particles/lit_particles_pool.hh"

namespace gem { namespace particle {
class LitParticleSystem : public ParticleSystem<LifeDeathCycle::Enabled,lit_particles_project::LitParticlesData> {
  DECLARE_UNCOPYABLE(LitParticleSystem)
  DECLARE_UNMOVABLE(LitParticleSystem)
public:
  LitParticleSystem(std::size_t a_unMaxParticleCount, std::size_t lightsCount,
    const std::string& a_sSystemName = std::move(std::string("DEFAULT_SYS_NAME")))
    : ParticleSystem<LifeDeathCycle::Enabled, lit_particles_project::LitParticlesData>(a_unMaxParticleCount, a_sSystemName) {
    m_pParticlePool->CreateLightIndexes(lightsCount);
    light::module::Resize(light::module::GetLightsCount() + lightsCount);
  }
  ~LitParticleSystem() = default;
}; /* class ParticlePool*/
/*}*/ /* namespace lit_particles_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: LIT_PARTICLES_SYSTEM_HH */
