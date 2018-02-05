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
#ifndef PARTICLE_ATTRACTOR
#define PARTICLE_ATTRACTOR

#include "dynamics/dynamic.hh"
#include "core/particle_pool_core.hh"

namespace gem {
namespace particle {
class ParticleAttractor : public Dynamic<CoreParticles> {
public:
  explicit ParticleAttractor(
    const glm::f32vec3 &a_fvAttractionPosition,
    float a_accelerationRate);
  ~ParticleAttractor() = default;

  void SetAttractorPosition(const glm::f32vec3& a_vPos) { m_fvAttractionPosition = a_vPos; }
  void SetAccelerationRate(float a_dRate) { m_fAccelerationRate = a_dRate; }
  
  float GetAccelerationRate() const { return m_fAccelerationRate; }
  glm::f32vec3 GetAttractorPosition() const { return m_fvAttractionPosition; }

  float* GetAccelerationRateRef() { return &m_fAccelerationRate; }
  glm::f32vec3* GetAttractorPositionRef() { return &m_fvAttractionPosition; }

  void Update(double a_dt, const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool) override;
private:
  glm::f32vec3	m_fvAttractionPosition;
  float         m_fAccelerationRate;
}; /* class ParticleAttractor*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: PARTICLE_ATTRACTOR */
