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
 * Inspired by: http://www.bfilipek.com/2014/04/flexible-particle-system-start.html
*************************************************************************/
#ifndef PARTICLE_POOL_CORE_HH
#define PARTICLE_POOL_CORE_HH

#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include "core/particle_pool.hh"
#include "core/particle_core_data.hh"
#include "utils/macro_definitions.hh"

namespace gem {
namespace particle {
using CoreParticles = void;
template <>
class ParticlePool<CoreParticles> {
  DECLARE_UNCOPYABLE(ParticlePool)
  DECLARE_UNMOVABLE(ParticlePool)
public: 
  /* Pool main principle/property:
  * Active particles are at the front (lower indexes) of the pool,
  * and sleeping particles are at the back (higher indexes) of the pool.
  * This prevents (de)allocation of particles or using a list of some sort
  * as suggested here http://gameprogrammingpatterns.com/object-pool.html.
  */
  std::unique_ptr<ParticleCoreData> pCoreData;
public:
  ParticlePool() = delete;
  explicit ParticlePool(std::size_t a_unMaxParticleCount)
    : pCoreData(new ParticleCoreData(a_unMaxParticleCount)),
      m_unParticleCount(a_unMaxParticleCount),
      m_unActiveParticleCount(0) {}
  ~ParticlePool() = default;
  
  inline std::size_t GetParticleCount() const {
    return m_unParticleCount;
  }
  inline std::size_t GetActiveParticleCount() const {
    return m_unActiveParticleCount;
  }
  // Particle activeness management methods
  void Sleep(std::size_t a_unParticleID) {
    // NOTE:  Should include the following check since 
    // no guarantee about ID and that activeparticlecount !=0
    // if (m_unActiveParticleCount > 0) 
    SwapPositions(a_unParticleID, m_unActiveParticleCount - 1);
    --m_unActiveParticleCount;
  }
  void Wake(std::size_t a_unParticleID) {
    // NOTE:  Should include the following check since 
    // no guarantee about ID and that activeparticlecount != m_vParticles.size()
    // if (m_unActiveParticleCount < m_vPool.size()) 
    //SwapPositions(a_unParticleID,m_unActiveParticleCount);
    ++m_unActiveParticleCount;
  }

private:
  void SwapPositions(
    std::size_t a_unFirstPosition,
    std::size_t a_unSecondPosition) {
    //TODO:Change the followings for a
    //simple copy with "=", since if
    // only sleep calls this, a_unFirstposition
    // data needs to be erased by 
    // a_unSecondPosition and that's it
    std::swap(pCoreData->m_lifetime[a_unFirstPosition],
      pCoreData->m_lifetime[a_unSecondPosition]);
    std::swap(pCoreData->m_color[a_unFirstPosition],
      pCoreData->m_color[a_unSecondPosition]);
    std::swap(pCoreData->m_position[a_unFirstPosition],
      pCoreData->m_position[a_unSecondPosition]);
    std::swap(pCoreData->m_velocity[a_unFirstPosition],
      pCoreData->m_velocity[a_unSecondPosition]);
  }

  std::size_t m_unParticleCount;
  std::size_t m_unActiveParticleCount;
}; /* class ParticlePool*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: PARTICLE_POOL_CORE_HH */
