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
#include "projects/lit_particles/lit_particles_emitter.hh"

#include <cstdlib>
#include <algorithm>

#include "utils/light_module.hh"
#include "utils/imgui/imgui_property_editor.h"

namespace gem { namespace particle {
namespace lit_particles_project {
const glm::vec4 LitParticlesEmitter::LIGHT_COLOR = { 0.5f, 0.5f, 0.5f, 0.5f };

float RandomFloat(float a_fMin, float a_fMax) {
  return a_fMin + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (a_fMax - a_fMin)));
}
LitParticlesEmitter::LitParticlesEmitter()
  : Emitter() { }

LitParticlesEmitter::LitParticlesEmitter(const glm::f32vec3& a_spawnLocation,
  const glm::f32vec3& a_spawnVelocity, float a_fLifetime, double a_dEmissionRate, 
  float a_fVelocityRandomization, const glm::u8vec4& a_color)
  : Emitter(a_spawnLocation, a_spawnVelocity, a_fLifetime, a_dEmissionRate),
    m_fVelocityRandomization(a_fVelocityRandomization), m_color(a_color) {
  // Add properties to the editor
  ImGuiPropertyEditor &editor = ImGuiPropertyEditor::GetInstance();
  editor.AddObject("Emitter", this);
  editor.AddProperty<PropertyType::VEC3>("Spawn velocity", &m_spawnVelocity, nullptr, 0.1f);
}

void LitParticlesEmitter::Init(double a_dt, const std::shared_ptr<ParticlePool<LitParticlesData> >& a_pPool,
  std::size_t a_unStartID, std::size_t a_unEndID) {
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pData->m_velocity[i] = RandomVelocity();
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pData->m_position[i] = m_spawnLocation;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pData->m_lifetime[i] = m_fLifetime;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    a_pPool->pData->m_color[i] = m_color;
  }
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    std::size_t wIndex = a_pPool->TakeLightIndex();
    if (wIndex <= light::module::MAX_LIGHTS) {
      a_pPool->pData->m_lightIndex[i] = wIndex;
      light::Light& wLight = light::module::GetLightRef(
        a_pPool->pData->m_lightIndex[i]);
      wLight.position = glm::vec4(m_spawnLocation, 0.0);
      wLight.color = LIGHT_COLOR;
      wLight.intensity = 0.1f;
      wLight.radius = 3.0f;
    }
  }
}

glm::f32vec3 LitParticlesEmitter::RandomVelocity() const {
  return m_spawnVelocity + glm::f32vec3(
    RandomFloat(-m_fVelocityRandomization, m_fVelocityRandomization),
    RandomFloat(-m_fVelocityRandomization, m_fVelocityRandomization),
    RandomFloat(-m_fVelocityRandomization, m_fVelocityRandomization)
  );
}
} /* namespace lit_particles_project */
} /* namespace particle */
} /* namespace gem */
