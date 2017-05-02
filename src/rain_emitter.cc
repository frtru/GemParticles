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
#include "rain_emitter.hh"

#include <cstdlib>
#include <algorithm>

namespace gem {
namespace particle {
namespace {
const float TWO_PI = 6.28319f;
const glm::f32vec3 RAIN_INITIAL_VELOCITY = { 2.0f, 0.0f, 0.2f };
const glm::u8vec4 RAIN_COLOR = { 0u,162u,232u,180u };
float RandomFloat(float a_fMin, float a_fMax) {
  return a_fMin + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (a_fMax - a_fMin)));
}

//TODO: If this gonna be reused, change the following so that
// it can be parametrized, random should be seeded, see if it can be
// optimized/changed for something cleaner/more C++11
glm::f32vec3 RandomInitialPosition() {
  // TODO: This could clearly be passed as a parameter at construction
  // but this is only a proof of concept, so not necessary to spend too
  // much time on this.
  return { RandomFloat(-100.0f, 100.0f), 100.0f + RandomFloat(-10.0f,10.0f), RandomFloat(-100.0f, 100.0f) };
}
}

RainEmitter::RainEmitter(float a_fLifetime, double a_dEmissionRate)
  : Emitter(glm::vec3(), glm::vec3(), a_fLifetime, a_dEmissionRate) {}

void RainEmitter::Init(double a_dt, const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool,
  std::size_t a_unStartID, std::size_t a_unEndID) {
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_velocity[i] = RAIN_INITIAL_VELOCITY;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_position[i] = RandomInitialPosition();
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_lifetime[i] = m_fLifetime;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pCoreData->m_color[i] = RAIN_COLOR;
  }
}
} /* namespace particle */
} /* namespace gem */
