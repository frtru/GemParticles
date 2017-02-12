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
#include "particle_system_component.hh"

#include "default_dynamic.hh"

namespace Gem {
namespace Particle {
ParticleSystemComponent::ParticleSystemComponent(
  std::size_t a_unMaxParticleCount,
  const std::string& a_sSystemName)
  : m_pParticlePool(new Pool(a_unMaxParticleCount)),
    m_sSystemName(a_sSystemName) {
  m_vDynamics.push_back(std::make_unique<DefaultDynamic>());
}
ParticleSystemComponent::ParticleSystemComponent(ParticleSystemComponent&& other)
  : m_pParticlePool(std::move(other.m_pParticlePool)),
  m_vDynamics(std::move(other.m_vDynamics)),
  m_vSources(std::move(other.m_vSources)) {
}
void ParticleSystemComponent::Update(double a_dt){
  for (auto& source : m_vSources) {
    source->Emit(a_dt, m_pParticlePool);
  }
  for (auto& dynamic : m_vDynamics) {
    dynamic->Update(a_dt, m_pParticlePool);
  }
}
} /* namespace Particle */
} /* namespace Gem */

