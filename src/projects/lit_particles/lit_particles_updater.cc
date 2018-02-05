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
#include "projects/lit_particles/lit_particles_updater.hh"

namespace gem { namespace particle {
namespace lit_particles_project {
void LitParticleUpdater::Update(double a_dt, const std::shared_ptr<ParticlePool<LitParticlesData> >& a_pPool) {
  // TODO: Deal with the delta double precision casted to float later
  // (GLM vec3 or vec4 doesn't support operations with doubles...)
  const float fDt = static_cast<float>(a_dt);

  for (std::size_t i = 0; i < a_pPool->GetActiveParticleCount(); ++i) {
    a_pPool->pData->m_lifetime[i] -= fDt;
    if (a_pPool->pData->m_lifetime[i] <= 0.0f) {
      // Do something with the lights before particle sleep
      a_pPool->ReleaseLightIndex(a_pPool->pData->m_lightIndex[i]);
      a_pPool->Sleep(i);
    }
  }

  for (std::size_t i = 0; i < a_pPool->GetActiveParticleCount(); ++i) {
    //Particle update
    a_pPool->pData->m_position[i] += a_pPool->pData->m_velocity[i] * fDt;

    //Light update
    light::Light& wLight = light::module::GetLightRef(
      a_pPool->pData->m_lightIndex[i]);
    wLight.position = glm::vec4(a_pPool->pData->m_position[i], 0.0);

    // End of life update
    if (a_pPool->pData->m_lifetime[i] <= .9f) {
      a_pPool->pData->m_color[i].a -= 4ui8;
      wLight.intensity -= 0.01; // TODO: test this once the rest works
      wLight.radius -= 0.04;
    }
  }
}
} /* namespace lit_particles_project */
} /* namespace particle */
} /* namespace gem */
