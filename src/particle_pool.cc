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
#include "particle_pool.hh"

namespace gem {
namespace particle {
Pool::Pool(std::size_t a_unMaxParticleCount)
  : m_lifetime(new float[a_unMaxParticleCount]),
    m_color(new glm::u8vec4[a_unMaxParticleCount]),
  m_position(new glm::f32vec3[a_unMaxParticleCount]),
  m_velocity(new glm::f32vec3[a_unMaxParticleCount]),
  m_unParticleCount(a_unMaxParticleCount),
  m_unActiveParticleCount(0) {}

void Pool::Sleep(std::size_t a_unParticleID) {
  // NOTE:  Should include the following check since 
  // no guarantee about ID and that activeparticlecount !=0
  // if (m_unActiveParticleCount > 0) 
  SwapPositions(a_unParticleID,m_unActiveParticleCount-1);
  --m_unActiveParticleCount;
}

// TODO: SwapPoolPositions should be needed only once,
// make sure it is not called twice per particle lifecycle
// (if placed correctly after sleep, no need to wake a random
// available particle just take the one at position
// m_unActiveParticleCount), in fact the wake function shouldn't
// need the parameter ID...
void Pool::Wake(std::size_t a_unParticleID) {
  // NOTE:  Should include the following check since 
  // no guarantee about ID and that activeparticlecount != m_vParticles.size()
  // if (m_unActiveParticleCount < m_vPool.size()) 
  //SwapPositions(a_unParticleID,m_unActiveParticleCount);
  ++m_unActiveParticleCount;
}

void Pool::SwapPositions(
    std::size_t a_unFirstPosition,
    std::size_t a_unSecondPosition) {
  //TODO:Change the followings for a
  //simple copy with "=", since if
  // only sleep calls this, a_unFirstposition
  // data needs to be erased by 
  // a_unSecondPosition and that's it
  std::swap(m_lifetime[a_unFirstPosition], 
    m_lifetime[a_unSecondPosition]);
  std::swap(m_color[a_unFirstPosition],
    m_color[a_unSecondPosition]);
  std::swap(m_position[a_unFirstPosition],
    m_position[a_unSecondPosition]);
  std::swap(m_velocity[a_unFirstPosition],
    m_velocity[a_unSecondPosition]);
}

//TODO: (NICE TO HAVE) The property to extend or reduce the capacity of the pool
//NOTE: Easy with the AoS approach
//void Pool::IncreaseParticleCount(std::size_t a_unAmount)
//{
//  m_vParticles.resize(m_vParticles.size() + a_unAmount);
//}
//
//void Pool::DecreaseParticleCount(std::size_t a_unAmount)
//{
//  m_vParticles.resize(m_vParticles.size() - a_unAmount);
//}
} /* namespace particle */
} /* namespace gem */