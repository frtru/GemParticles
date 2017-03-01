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

#include <glm/glm.hpp>

#include "particle_pool.hh"
#include "macro_definitions.hh"

namespace gem {
namespace particle {
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
	Emitter();
	Emitter(
    const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime,
    double a_dEmissionRate);
	virtual ~Emitter() = default;

  void SetSpawnLocation(const glm::f32vec3& a_spawnLocation);
  void SetSpawnVelocity(const glm::f32vec3& a_spawnVelocity);
  void SetParticleLifetime(float a_fLifetime);
  void SetEmissionRate(double a_dEmissionRate);

  /*TODO: When you'll be at the point to set the rate through
  the UI, it will be the time start caring about those things
  double GetEmissionRate() const { return m_dEmissionRate; }
  void SetEmissionRate(double a_dEmissionRate) {
    m_dEmissionRate = a_dEmissionRate;
  }*/

  void Emit(double a_dt, const std::shared_ptr<ParticlePool>& a_pPool);

protected:
  float         m_fLifetime;
  double        m_dEmissionRate;
  glm::f32vec3  m_spawnLocation;
  glm::f32vec3  m_spawnVelocity;

private:
  // Private initialization of the particles before emission
  // in the subclasses
  virtual void Init(double a_dt, const std::shared_ptr<ParticlePool>& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) = 0;
}; /* class Emitter*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: EMITTER_HH */
