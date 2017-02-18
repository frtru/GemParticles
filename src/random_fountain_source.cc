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
#include "random_fountain_source.hh"

#include <cstdlib>
#include <algorithm>

namespace gem {
namespace particle {
namespace {
const float TWO_PI = 6.28319f;
const glm::u8vec4 FOUNTAIN_COLOR = { 0u,162u,232u,180u };
float RandomFloat(float a_fMin, float a_fMax) {
  return a_fMin + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (a_fMax - a_fMin)));
}

//TODO: If this gonna be reused, change the following so that
// it can be paramterized, random should be seeded, see if it can be
// optimized/changed for something cleaner/more C++11
const glm::f32vec3 RandomCircularVelocity() {
  float theta = RandomFloat(0.0f, TWO_PI);
  float dist  = RandomFloat(0.0f, 2.0f);
  return { dist*cos(theta), 2.0f, dist*sin(theta) };
}
}

RandomFountainSource::RandomFountainSource(const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime, 
    double a_dEmissionRate)
  : Source(a_spawnLocation,
  a_spawnVelocity,
  a_fLifetime,
  a_dEmissionRate) {}

void RandomFountainSource::Init(double a_dt, const std::unique_ptr<Pool>& a_pPool,
  std::size_t a_unStartID, std::size_t a_unEndID) {
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->m_velocity[i]      = RandomCircularVelocity();
    a_pPool->m_position[i]      = m_spawnLocation;
    a_pPool->m_lifetime[i]      = m_fLifetime; 
    a_pPool->m_color[i]         = FOUNTAIN_COLOR; 
  }
}
} /* namespace particle */
} /* namespace gem */
