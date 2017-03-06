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

/* TODOs
 * 2- Since we have the lifetime > 0.0 to see if particles are active,
 * an active particle counter and the pool property of having active particles
 * at the front, couldn't we just get rid of the activeflags container??
 * Maybe performance wise it is just safer to have one...TBD
 * 3- Try AoS vs SoA (in other words move the particle parameters here
 * and create an array for each
 * 4- See unique_ptr vs vector for pool
 */

#ifndef PARTICLE_POOL_HH
#define PARTICLE_POOL_HH

#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include "particle_core_data.hh"
#include "macro_definitions.hh"

namespace gem {
namespace particle {
class ParticlePool {
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
  explicit ParticlePool(std::size_t a_unMaxParticleCount);
  ~ParticlePool() = default;
  
  std::size_t GetParticleCount() const {
    return m_unParticleCount;
  }
  std::size_t GetActiveParticleCount() const {
    return m_unActiveParticleCount;
  }
  // Particle activeness management methods
  void Sleep(std::size_t a_unParticleID);
  void Wake(std::size_t a_unParticleID);

private:
  void SwapPositions(
      std::size_t a_unFirstPosition,
      std::size_t a_unSecondPosition);

  std::size_t m_unParticleCount;
  std::size_t m_unActiveParticleCount;
}; /* class ParticlePool*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: PARTICLE_POOL_HH */
