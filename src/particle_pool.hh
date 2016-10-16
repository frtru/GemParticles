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

#include "particle.hh"

namespace Gem {
namespace Particle {
class Pool {
public: 
  /* Pool main principle/property:
  * Active particles are at the front (lower indexes) of the pool,
  * and sleeping particles are at the back (higher indexes) of the pool.
  * This prevents (de)allocation of particles or using a list of some sort
  * as suggested here http://gameprogrammingpatterns.com/object-pool.html.
  */

  // TODO: See if alpha is really required here.
  // Every particles of the same system is going
  // to have the same alpha value probably.
  // It's only 1 byte though and for alignement
  // it might be useful to keep it.
  std::unique_ptr<float[]       > m_lifetime;
  std::unique_ptr<glm::u8vec4[] >	m_color;
  //TODO: See if following is necessary
//  std::unique_ptr<glm::u8vec4[] >	m_startColor;
//  std::unique_ptr<glm::u8vec4[] >	m_endColor;
  std::unique_ptr<glm::f32vec3[]>	m_position;
  std::unique_ptr<glm::f32vec3[]>	m_velocity;
  std::unique_ptr<glm::f32vec3[]>	m_acceleration;
public:
  Pool() = delete;
  explicit Pool(std::size_t a_unMaxParticleCount);

  Pool(Pool&& other) = delete;
  Pool(const Pool& other) = delete;
  Pool& operator=(Pool&& other) = delete;
  Pool& operator=(const Pool& other) = delete;

  ~Pool();
  
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
} /* class Pool*/;
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: PARTICLE_POOL_HH */
