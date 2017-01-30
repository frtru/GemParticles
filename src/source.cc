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
#include "source.hh"

#include <algorithm>

namespace Gem {
namespace Particle {
glm::f32vec3 const Source::ORIGIN           = { 0.0f, 0.0f, 0.0f };
glm::f32vec3 const Source::DEFAULT_SPEED    = { 0.0f, 2.0f, 0.0f };
float const Source::DEFAULT_LIFETIME        = 1.0f;
double const Source::DEFAULT_EMISSION_RATE  = 100000.0;

// Default redish transparent color 
// overwritten by updaters start/end color
glm::u8vec4  const Source::DEFAULT_COLOR    = { 0u,255u,0u,180u };

Source::Source() 
  : m_spawnLocation(ORIGIN),
  m_spawnVelocity(DEFAULT_SPEED),
  m_fLifetime(DEFAULT_LIFETIME),
  m_dEmissionRate(DEFAULT_EMISSION_RATE) {}

Source::Source(const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime,
    double a_dEmissionRate)
  : m_spawnLocation(a_spawnLocation),
  m_spawnVelocity(a_spawnVelocity),
  m_fLifetime(a_fLifetime),
  m_dEmissionRate(a_dEmissionRate) {}

void Source::SetSpawnLocation(const glm::f32vec3& a_spawnLocation) {
  m_spawnLocation = a_spawnLocation;
}

void Source::SetSpawnVelocity(const glm::f32vec3& a_spawnVelocity) {
  m_spawnVelocity = a_spawnVelocity;
}

void Source::SetParticleLifetime(float a_fLifetime) {
  m_fLifetime = a_fLifetime;
}

void Source::SetEmissionRate(double a_dEmissionRate) {
  m_dEmissionRate = a_dEmissionRate;
}


void Source::Emit(double a_dt, const std::unique_ptr<Pool>& a_pPool) {
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
} /* namespace Particle */
} /* namespace Gem */
