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
#include "dynamics/particle_attractor.hh"

#include "utils/imgui/imgui_property_editor.h"

namespace gem {
namespace particle {
ParticleAttractor::ParticleAttractor(const glm::f32vec3 &a_fvAttractionPosition, float a_accelerationRate)
  : m_fvAttractionPosition(a_fvAttractionPosition), m_fAccelerationRate(a_accelerationRate) {
  // Adding the properties of this component to the editor
  ImGuiPropertyEditor &editor = ImGuiPropertyEditor::GetInstance();
  editor.AddObject("Particle attractor", this);
  editor.AddProperty<PropertyType::INPUT_FLOAT>("Acceleration rate", &m_fAccelerationRate,nullptr,0.05f);
  editor.AddProperty<PropertyType::VEC3>("Attraction position", &m_fvAttractionPosition);
}

void ParticleAttractor::Update(double a_dt, 
  const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool) {
  const float   fDt = static_cast<float>(a_dt);
  const float   fIterationAccel = fDt * m_fAccelerationRate;
  glm::f32vec3  fvAccelDir;
  for (std::size_t i = 0; i < a_pPool->GetActiveParticleCount(); ++i) {
    //fvAccelDir = m_fvAttractionPosition - a_pPool->pCoreData->m_position[i]; // Makes all the particles move together because the acceleration vector is bigger for farther particles
    fvAccelDir = glm::normalize(m_fvAttractionPosition - a_pPool->pCoreData->m_position[i]);
    a_pPool->pCoreData->m_velocity[i] += fvAccelDir * fIterationAccel;
  }
  
  for (std::size_t i = 0; i < a_pPool->GetActiveParticleCount(); ++i) {
    a_pPool->pCoreData->m_position[i] += a_pPool->pCoreData->m_velocity[i] * fDt;
  }
}
} /* namespace particle */
} /* namespace gem */
