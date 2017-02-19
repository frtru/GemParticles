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

#include "euler_particle_updater.hh"

namespace gem {
namespace particle {
ParticleSystemComponent::ParticleSystemComponent(
  std::size_t a_unMaxParticleCount)
  : m_pParticlePool(std::make_shared<ParticlePool>(a_unMaxParticleCount)){
  m_vDynamics.push_back(std::make_unique<EulerParticleUpdater>());
}
void ParticleSystemComponent::Update(double a_dt){
  for (auto& emmiter : m_vEmitters) {
    emmiter->Emit(a_dt, m_pParticlePool);
  }
  for (auto& emmiter : m_vDynamics) {
    emmiter->Update(a_dt, m_pParticlePool);
  }
}
} /* namespace particle */
} /* namespace gem */

