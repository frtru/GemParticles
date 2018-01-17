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
#include "projects/lit_particles/ground_collision.hh"

namespace gem {
namespace particle {
void GroundCollision::Update(double a_dt, const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool) {
  // TODO: Deal with the delta double precision casted to float later
  // (GLM vec3 or vec4 doesn't support operations with doubles...)
  const float fDt = static_cast<float>(a_dt);

  // Using the euler model to update the positions and velocities
  for (std::size_t i = 0; i < a_pPool->GetActiveParticleCount(); ++i) {
    if (a_pPool->pCoreData->m_position[i].y < -0.9f)
      a_pPool->pCoreData->m_velocity[i].y *= -0.5f;
  }
}
} /* namespace particle */
} /* namespace gem */
