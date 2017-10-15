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
#ifndef AVX_PARTICLE_ATTRACTOR
#define AVX_PARTICLE_ATTRACTOR

#include "dynamics/dynamic.hh"
#include "core/particle_pool_core.hh"

namespace gem {
namespace particle {
class AVXParticleAttractor : public Dynamic<CoreParticles> {
public:
  explicit AVXParticleAttractor(
    const glm::f32vec3 &a_fvAttractionPosition,
    float a_accelerationRate);
  ~AVXParticleAttractor() = default;
  
  void Update(double a_dt, const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool) override;
private:
	glm::f32vec3	m_fvAttractionPosition;
  float         m_fAccelerationRate;
}; /* class AVXParticleAttractor */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: AVX_PARTICLE_ATTRACTOR */
