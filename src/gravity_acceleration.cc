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
#include "gravity_acceleration.hh"

namespace gem {
namespace particle {
const glm::f32vec3 GravityAcceleration::GRAVITY_ACCEL = { 0.0f,-9.80665f,0.0f };

void GravityAcceleration::Update(double a_dt, 
  const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool) {
  // (GLM vec3 or vec4 doesn't support operations with doubles...)
  const float fDt = static_cast<float>(a_dt);

  for (int i = 0; i < a_pPool->GetActiveParticleCount(); ++i) {
    a_pPool->pCoreData->m_velocity[i] += GRAVITY_ACCEL * fDt;
  }
}
} /* namespace particle */
} /* namespace gem */
