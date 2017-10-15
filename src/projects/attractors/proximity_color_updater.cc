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

namespace gem { namespace particle {
namespace attractor_project {
namespace {
// Helper function for u8vec4 and float multiplication
glm::u8vec4 multiply(float f, glm::u8vec4 v) {
  v.r *= f;  v.g *= f;  v.b *= f;  v.a *= f;
  return v;
}
}
ProximityColorUpdater::ProximityColorUpdater(
  const glm::f32vec3 &a_fvPositionOfInterest,
  const glm::u8vec4 &a_fvHotColor,
  const glm::u8vec4 &a_fvColdColor,
  float a_fMaxDistance)
  : m_fvPositionOfInterest(a_fvPositionOfInterest),
    m_fMaxDistance(a_fMaxDistance) {
  m_vColorGradient.reserve(a_fMaxDistance+1);
  glm::u8vec4 wColor;
  float wfDistance = 0.0f;
  for (float i = 0u; i <= a_fMaxDistance; ++i) {
    wfDistance = i/a_fMaxDistance;
    wColor = multiply(1- wfDistance, a_fvHotColor) + multiply(wfDistance, a_fvColdColor);
    m_vColorGradient.emplace_back(wColor);
  }
}

void ProximityColorUpdater::Update(double a_dt,
  const std::shared_ptr<ParticlePool<CoreParticles> >& a_pPool) {
  const float   fDt = static_cast<float>(a_dt);
  float         fDistance = 0.0f;
  for (std::size_t i = 0; i < a_pPool->GetActiveParticleCount(); ++i) { 
    fDistance = std::min(glm::distance(a_pPool->pCoreData->m_position[i], m_fvPositionOfInterest), m_fMaxDistance);
    a_pPool->pCoreData->m_color[i] = m_vColorGradient[static_cast<std::size_t>(fDistance)];
  }
}
} /* namespace attractor_project */
} /* namespace particle */
} /* namespace gem */
