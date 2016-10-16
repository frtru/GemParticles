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
#include "fountain_source.hh"

#include <algorithm>

namespace Gem {
namespace Particle {
FountainSource::FountainSource(const glm::f32vec3& a_spawnLocation,/*
  const glm::f32vec3& a_spawnVelocity,
  const glm::f32vec3& a_spawnAcceleration,
  const glm::u8vec4& a_startColor,
  const glm::u8vec4& a_EndColor,
  float lifetime = 1.0f,*/
  double a_dEmissionRate)
  : ISource(a_dEmissionRate),
    m_spawnLocation(a_spawnLocation) {
  // TODO: Every parameter except for the acceleration
  // should be received from external party
}

FountainSource::~FountainSource() {}

void FountainSource::Init(double a_dt, const std::unique_ptr<Pool>& a_pPool,
  std::size_t a_unStartID, std::size_t a_unEndID) {
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    // TODO: See if += is possible and if 
    // an affectation like that overwrite/overlap with 
    // other sources work
    a_pPool->m_acceleration[i]  = GRAVITY_ACCEL;
    a_pPool->m_velocity[i]      = { 0.0f,0.0f,0.0f };
    a_pPool->m_position[i]      = m_spawnLocation;
    a_pPool->m_lifetime[i]      = 0.0f; //TODO: temporary, replace with m_lifetime;
    a_pPool->m_color[i]         = { 0.0f,0.0f,0.0f,0.0f }; //TODO: temporary, replace with m_color
  }
}
} /* namespace Particle */
} /* namespace Gem */
