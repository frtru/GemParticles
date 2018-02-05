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
#ifndef LIT_PARTICLES_EMITTER_HH
#define LIT_PARTICLES_EMITTER_HH

#include "emitters/emitter.hh"
#include "projects/lit_particles/lit_particles_pool.hh"

namespace gem { namespace particle {
namespace lit_particles_project {
class LitParticlesEmitter : public Emitter<LitParticlesData> {
private:
  static const glm::vec4 LIGHT_COLOR;
public:
  LitParticlesEmitter();
  LitParticlesEmitter(
    const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime,
    double a_dEmissionRate,
    float a_dVelocityRandomization = 0.0f,
    const glm::u8vec4& a_color = DEFAULT_COLOR);
	virtual ~LitParticlesEmitter() = default;

private:
  void Init(double a_dt, const std::shared_ptr<ParticlePool<LitParticlesData> >& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) override;
  glm::f32vec3 RandomVelocity() const;

  float       m_fVelocityRandomization;
  glm::u8vec4 m_color;
}; /* class LitParticlesEmitter*/
} /* namespace lit_particles_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: LIT_PARTICLES_EMITTER_HH */
