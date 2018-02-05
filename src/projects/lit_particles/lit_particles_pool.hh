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
*************************************************************************/
#ifndef LIT_PARTICLES_POOL_HH
#define LIT_PARTICLES_POOL_HH

#include <deque>
#include <memory>

#include "glm/glm.hpp"

#include "core/particle_pool.hh"
#include "projects/lit_particles/lit_particles_data.hh"
#include "utils/macro_definitions.hh"
#include "utils/light_module.hh"

namespace gem { namespace particle {

//  According to C++ Standard 14.7.3 / 2:
//An explicit specialization shall be declared in the namespace 
//of which the template is a member, or , for member templates, 
//in the namespace of which the enclosing class or enclosing class 
//template is a member.

//namespace lit_particles_project {
template <>
class ParticlePool<lit_particles_project::LitParticlesData> {
  DECLARE_UNCOPYABLE(ParticlePool)
  DECLARE_UNMOVABLE(ParticlePool)
public: 
  /* Pool main principle/property:
  * Active particles are at the front (lower indexes) of the pool,
  * and sleeping particles are at the back (higher indexes) of the pool.
  * This prevents (de)allocation of particles or using a list of some sort
  * as suggested here http://gameprogrammingpatterns.com/object-pool.html.
  */
  std::unique_ptr<lit_particles_project::LitParticlesData> pData;
public:
  ParticlePool() = delete;
  explicit ParticlePool(std::size_t a_unMaxParticleCount)
    : pData(new lit_particles_project::LitParticlesData(a_unMaxParticleCount)),
      m_unParticleCount(a_unMaxParticleCount),
      m_unActiveParticleCount(0) { }
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

  void CreateLightIndexes(std::size_t indexesCount) {
    std::size_t wLightCount = light::module::GetLightsCount(); // wLightCount as an index points after the last light
    for (std::size_t i = 0; i < indexesCount; ++i, ++wLightCount) {
      dAvailableIndexes.push_back(wLightCount);
    }
  }

  std::size_t TakeLightIndex() {
    if (dAvailableIndexes.empty())
      return -1;
    std::size_t wReturnIndex = dAvailableIndexes.front();
    dAvailableIndexes.pop_front();
    return wReturnIndex;
  }

  void ReleaseLightIndex(std::size_t index) {
    // TODO: Maybe would be useful to add a check to see if the index is already in the deque
    dAvailableIndexes.push_back(index);
  }

private:
  void SwapPositions(
    std::size_t a_unFirstPosition,
    std::size_t a_unSecondPosition) {
    std::swap(pData->m_lifetime[a_unFirstPosition],
      pData->m_lifetime[a_unSecondPosition]);
    std::swap(pData->m_color[a_unFirstPosition],
      pData->m_color[a_unSecondPosition]);
    std::swap(pData->m_position[a_unFirstPosition],
      pData->m_position[a_unSecondPosition]);
    std::swap(pData->m_velocity[a_unFirstPosition],
      pData->m_velocity[a_unSecondPosition]);
    std::swap(pData->m_lightIndex[a_unFirstPosition],
      pData->m_lightIndex[a_unSecondPosition]);
  }

  std::size_t m_unParticleCount;
  std::size_t m_unActiveParticleCount;
  std::deque<std::size_t> dAvailableIndexes;
}; /* class ParticlePool*/
/*}*/ /* namespace lit_particles_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: LIT_PARTICLES_POOL_HH */
