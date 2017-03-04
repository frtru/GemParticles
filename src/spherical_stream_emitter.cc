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
#include "spherical_stream_emitter.hh"

#include <algorithm>
#include<random>
#include<cmath>
#include<chrono>
#include <vector>

namespace gem {
namespace particle {

namespace {
std::vector<glm::f32vec3> GenerateSphericalBoundCoordinates(
  // TODO: Solve this : not really being spawned in a spherical shape
  const glm::f32vec3& a_CenterCoordinate,
  float a_fRadius,
  std::size_t a_unNumberOfCoords) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);
  std::uniform_real_distribution<float> uniform01(0.0f, 1.0f);

  std::vector<glm::f32vec3> coords;
  coords.reserve(a_unNumberOfCoords);
  
  const float PI = 3.14159;
  for (std::size_t i = 0ULL; i < a_unNumberOfCoords; i++) {
    float theta = 2 * PI * uniform01(generator);
    float phi = acos(1 - 2 * uniform01(generator));
    glm::f32vec3 coord;
    coord[0] = a_fRadius * sin(phi) * cos(theta);
    coord[1] = a_fRadius * sin(phi) * sin(theta);
    coord[2] = a_fRadius * cos(phi);
    coords.push_back(std::move(coord));
  }
  return coords;
}
}

SphericalStreamEmitter::SphericalStreamEmitter(
    const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime, 
    double a_dEmissionRate)
  : Emitter(a_spawnLocation,
  a_spawnVelocity,
  a_fLifetime,
  a_dEmissionRate) {}

void SphericalStreamEmitter::Init(double a_dt, const std::shared_ptr<ParticlePool>& a_pPool,
  std::size_t a_unStartID, std::size_t a_unEndID) {
  const std::size_t N = a_unEndID - a_unStartID;
  auto coords = GenerateSphericalBoundCoordinates(
    m_spawnLocation,
    0.2f, N);

  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->m_velocity[i] = m_spawnVelocity;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->m_position[i] = coords[i - a_unStartID];
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->m_lifetime[i] = m_fLifetime;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->m_color[i] = DEFAULT_COLOR;
  }
}
} /* namespace particle */
} /* namespace gem */
