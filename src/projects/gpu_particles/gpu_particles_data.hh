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
#ifndef LIT_PARTICLES_DATA_HH
#define LIT_PARTICLES_DATA_HH

#include <memory>
#include "glm/glm.hpp"

namespace gem { namespace particle {
namespace gpu_particles_project {
struct LitParticlesData {
  std::unique_ptr<float[]       > m_lifetime;
  std::unique_ptr<glm::u8vec4[] >	m_color;
  std::unique_ptr<glm::f32vec3[]>	m_position;
  std::unique_ptr<glm::f32vec3[]>	m_velocity;
  std::unique_ptr<std::size_t[] > m_lightIndex;

  explicit LitParticlesData(std::size_t a_unMaxParticleCount)
    : m_lifetime(new float[a_unMaxParticleCount]),
      m_color(new glm::u8vec4[a_unMaxParticleCount]),
      m_position(new glm::f32vec3[a_unMaxParticleCount]),
      m_velocity(new glm::f32vec3[a_unMaxParticleCount]),
      m_lightIndex(new std::size_t[a_unMaxParticleCount]) {}
}; /* class LitParticlesData */
} /* namespace gpu_particles_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: LIT_PARTICLES_DATA_HH */
