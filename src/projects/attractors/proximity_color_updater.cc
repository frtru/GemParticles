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
#include "projects/attractors/proximity_color_updater.hh"

#include <algorithm>
#include <immintrin.h>
#include "glm/glm.hpp"

#include "utils/imgui/imgui_property_editor.h"

namespace gem { namespace particle {
namespace attractor_project {
namespace {
// Helper function for u8vec4 and float multiplication
//glm::u8vec4 multiply(float f, glm::u8vec4 v) {
//  v.r *= f;  v.g *= f;  v.b *= f;  v.a *= f;
//  return v;
//}

glm::u8vec4 vec4Tou8Vec4(const glm::vec4& v) {
  return {
    v.r * 255u,
    v.g * 255u,
    v.b * 255u,
    v.a * 255u
  };
}
}
ProximityColorUpdater::ProximityColorUpdater(
  const glm::f32vec3 &a_fvPositionOfInterest,
  const glm::vec4 &a_fvHotColor,
  const glm::vec4 &a_fvColdColor,
  float a_fMaxDistance)
  : m_fvPositionOfInterest(a_fvPositionOfInterest),
    m_fMaxDistance(a_fMaxDistance),
    m_vColdColor(new glm::vec4(a_fvColdColor)),
    m_vHotColor(new glm::vec4(a_fvHotColor)) {
  m_vColorGradient.reserve(a_fMaxDistance+1);
  glm::vec4 wColor;
  float wfDistance = 0.0f;
  for (float i = 0u; i <= a_fMaxDistance; ++i) {
    wfDistance = i/a_fMaxDistance;
    wColor = (1 - wfDistance)*a_fvHotColor + wfDistance*a_fvColdColor;
    m_vColorGradient.emplace_back(wColor);
  }

  // Adding the properties of this component to the editor
  ImGuiPropertyEditor &editor = ImGuiPropertyEditor::GetInstance();
  editor.AddObject("Proximity Color Updater", this);
  editor.AddProperty("Max distance", EditableProperty::DRAG_FLOAT, 0, &m_fMaxDistance);
  editor.AddProperty("Hot color",   EditableProperty::COLOR, 1, m_vHotColor);
  editor.AddProperty("Cold color",  EditableProperty::COLOR, 2, m_vColdColor);
}

ProximityColorUpdater::~ProximityColorUpdater() {
  delete m_vColdColor;
  delete m_vHotColor;
}

void ProximityColorUpdater::UpdateColorGradient() {
  glm::vec4 wColor;
  float wfDistance = 0.0f;
  for (float i = 0u; i <= m_fMaxDistance; ++i) {
    wfDistance = i / m_fMaxDistance;
    wColor = (1 - wfDistance)*(*m_vHotColor) + wfDistance*(*m_vColdColor);
    m_vColorGradient[i] = wColor;
  }
}

void ProximityColorUpdater::Update(double a_dt,
  const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool) {
  const float   fDt = static_cast<float>(a_dt);
  float         fDistance = 0.0f;
  for (std::size_t i = 0; i < a_pPool->GetActiveParticleCount(); ++i) { 
    fDistance = std::min(glm::distance(a_pPool->pCoreData->m_position[i], m_fvPositionOfInterest), m_fMaxDistance);
    a_pPool->pCoreData->m_color[i] = vec4Tou8Vec4(m_vColorGradient[static_cast<std::size_t>(fDistance)]);
  }
}
} /* namespace attractor_project */
} /* namespace particle */
} /* namespace gem */
