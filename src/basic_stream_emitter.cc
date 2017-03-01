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
#include "basic_stream_emitter.hh"

#include <algorithm>

namespace gem {
namespace particle {
BasicStreamEmitter::BasicStreamEmitter()
  : Emitter() {}

BasicStreamEmitter::BasicStreamEmitter(
  const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime, 
    double a_dEmissionRate)
  : Emitter(a_spawnLocation,
  a_spawnVelocity,
  a_fLifetime,
  a_dEmissionRate) {}

void BasicStreamEmitter::Init(double a_dt, const std::shared_ptr<ParticlePool>& a_pPool,
  std::size_t a_unStartID, std::size_t a_unEndID) {
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->m_velocity[i]      = m_spawnVelocity;
    a_pPool->m_position[i]      = m_spawnLocation;
    a_pPool->m_lifetime[i]      = m_fLifetime; 
    a_pPool->m_color[i]         = DEFAULT_COLOR;
  }
}
} /* namespace particle */
} /* namespace gem */
