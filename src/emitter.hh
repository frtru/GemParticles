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
#ifndef EMITTER_HH
#define EMITTER_HH

#include <algorithm>
#include <memory>

#include <glm/glm.hpp>

#include "particle_pool.hh"
#include "macro_definitions.hh"

namespace gem {
namespace particle {
template <class ParticleType>
class Emitter {
  // This would normally be a copyable object, but the
  // only advantage would be to be able to duplicate emitters...
  // I prefer avoiding implicit unwanted constructions
  DECLARE_UNCOPYABLE(Emitter)
  DECLARE_UNMOVABLE(Emitter)
private:
  static const glm::f32vec3 ORIGIN;
  static const glm::f32vec3 DEFAULT_SPEED;
  static const float        DEFAULT_LIFETIME;
  static const double       DEFAULT_EMISSION_RATE;
protected:
  static const glm::u8vec4  DEFAULT_COLOR;

public:
	Emitter()
    : m_spawnLocation(ORIGIN),
      m_spawnVelocity(DEFAULT_SPEED),
      m_fLifetime(DEFAULT_LIFETIME),
      m_dEmissionRate(DEFAULT_EMISSION_RATE) {}
	Emitter(const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime,
    double a_dEmissionRate)
    : m_spawnLocation(a_spawnLocation),
      m_spawnVelocity(a_spawnVelocity),
      m_fLifetime(a_fLifetime),
      m_dEmissionRate(a_dEmissionRate) {}
	virtual ~Emitter() = default;

  inline void SetSpawnLocation(const glm::f32vec3& a_spawnLocation) {
    m_spawnLocation = a_spawnLocation;
  }
  inline void SetSpawnVelocity(const glm::f32vec3& a_spawnVelocity) {
    m_spawnVelocity = a_spawnVelocity;
  }
  inline void SetParticleLifetime(float a_fLifetime) {
    m_fLifetime = a_fLifetime;
  }
  inline void SetEmissionRate(double a_dEmissionRate) {
    m_dEmissionRate = a_dEmissionRate;
  }

  virtual void Emit(double a_dt, const std::shared_ptr<ParticlePool<ParticleType> >& a_pPool) {
    const std::size_t maxNewParticles     = static_cast<std::size_t>(a_dt*m_dEmissionRate);
    const std::size_t firstParticleIndex  = a_pPool->GetActiveParticleCount();
    const std::size_t lastParticleIndex   = std::min(firstParticleIndex + maxNewParticles,
      a_pPool->GetParticleCount() - 1);

    Init(a_dt, a_pPool, firstParticleIndex, lastParticleIndex);

    for (std::size_t i = firstParticleIndex; i < lastParticleIndex; ++i) {
      a_pPool->Wake(i);
    }
  }

protected:
  float         m_fLifetime;
  double        m_dEmissionRate;
  glm::f32vec3  m_spawnLocation;
  glm::f32vec3  m_spawnVelocity;

private:
  // Private initialization of the particles before emission
  // in the subclasses
  virtual void Init(double a_dt, const std::shared_ptr<ParticlePool<ParticleType> >& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) = 0;
}; /* class Emitter*/

template <class ParticleType>
const glm::f32vec3 Emitter<ParticleType>::ORIGIN = { 0.0f, 0.0f, 0.0f };
template <class ParticleType>
const glm::f32vec3 Emitter<ParticleType>::DEFAULT_SPEED = { 0.5f, 2.0f, 0.0f };
template <class ParticleType>
const float Emitter<ParticleType>::DEFAULT_LIFETIME = 1.0f;
template <class ParticleType>
const double Emitter<ParticleType>::DEFAULT_EMISSION_RATE = 100000.0;
// Default redish transparent color 
// overwritten by updaters start/end color
template <class ParticleType>
const glm::u8vec4 Emitter<ParticleType>::DEFAULT_COLOR = { 255u,0u,0u,180u };
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: EMITTER_HH */
