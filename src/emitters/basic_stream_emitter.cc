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
#include "emitters/basic_stream_emitter.hh"

#include <cstdlib>
#include <algorithm>

namespace gem {
namespace particle {
float RandomFloat(float a_fMin, float a_fMax) {
  return a_fMin + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (a_fMax - a_fMin)));
}

BasicStreamEmitter::BasicStreamEmitter()
  : Emitter() {}

BasicStreamEmitter::BasicStreamEmitter(const glm::f32vec3& a_spawnLocation,
  const glm::f32vec3& a_spawnVelocity, float a_fLifetime, double a_dEmissionRate, 
  float a_fVelocityRandomization, const glm::u8vec4& a_color)
  : Emitter(a_spawnLocation, a_spawnVelocity, a_fLifetime, a_dEmissionRate),
    m_fVelocityRandomization(a_fVelocityRandomization), m_color(a_color) { }

void BasicStreamEmitter::Init(double a_dt, const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool,
  std::size_t a_unStartID, std::size_t a_unEndID) {
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_velocity[i] = RandomVelocity();
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_position[i] = m_spawnLocation;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_lifetime[i] = m_fLifetime;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_color[i] = m_color;
  }
}

glm::f32vec3 BasicStreamEmitter::RandomVelocity() const {
  return m_spawnVelocity + glm::f32vec3(
    RandomFloat(-m_fVelocityRandomization, m_fVelocityRandomization),
    RandomFloat(-m_fVelocityRandomization, m_fVelocityRandomization),
    RandomFloat(-m_fVelocityRandomization, m_fVelocityRandomization)
  );
}
} /* namespace particle */
} /* namespace gem */
