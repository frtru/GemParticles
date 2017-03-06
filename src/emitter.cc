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
#include "emitter.hh"

#include <algorithm>

namespace gem {
namespace particle {
glm::f32vec3 const Emitter::ORIGIN           = { 0.0f, 0.0f, 0.0f };
glm::f32vec3 const Emitter::DEFAULT_SPEED    = { 0.5f, 2.0f, 0.0f };
float const Emitter::DEFAULT_LIFETIME        = 1.0f;
double const Emitter::DEFAULT_EMISSION_RATE  = 100000.0;

// Default redish transparent color 
// overwritten by updaters start/end color
glm::u8vec4  const Emitter::DEFAULT_COLOR    = { 255u,0u,0u,180u };

Emitter::Emitter() 
  : m_spawnLocation(ORIGIN),
  m_spawnVelocity(DEFAULT_SPEED),
  m_fLifetime(DEFAULT_LIFETIME),
  m_dEmissionRate(DEFAULT_EMISSION_RATE) {}

Emitter::Emitter(const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime,
    double a_dEmissionRate)
  : m_spawnLocation(a_spawnLocation),
  m_spawnVelocity(a_spawnVelocity),
  m_fLifetime(a_fLifetime),
  m_dEmissionRate(a_dEmissionRate) {}

void Emitter::SetSpawnLocation(const glm::f32vec3& a_spawnLocation) {
  m_spawnLocation = a_spawnLocation;
}

void Emitter::SetSpawnVelocity(const glm::f32vec3& a_spawnVelocity) {
  m_spawnVelocity = a_spawnVelocity;
}

void Emitter::SetParticleLifetime(float a_fLifetime) {
  m_fLifetime = a_fLifetime;
}

void Emitter::SetEmissionRate(double a_dEmissionRate) {
  m_dEmissionRate = a_dEmissionRate;
}


void Emitter::Emit(double a_dt, const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool) {
  const std::size_t maxNewParticles = 
    static_cast<std::size_t>(a_dt*m_dEmissionRate);
  const std::size_t firstParticleIndex = 
    a_pPool->GetActiveParticleCount();
  const std::size_t lastParticleIndex = std::min(
    firstParticleIndex + maxNewParticles, 
    a_pPool->GetParticleCount() - 1);

  Init(a_dt, a_pPool, firstParticleIndex, lastParticleIndex);

  for (std::size_t i = firstParticleIndex; i < lastParticleIndex; ++i) {
    a_pPool->Wake(i);
  }
}
} /* namespace particle */
} /* namespace gem */
