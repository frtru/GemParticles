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
#include "particle_system.hh"

namespace gem {
namespace particle {
ParticleSystem::ParticleSystem(
  std::unique_ptr<ParticleSystemComponent> &&a_pComponent,
  std::unique_ptr<Renderer> &&a_pRenderer,
  std::string &&a_sSystemName)
  : m_pComponent(std::move(a_pComponent)),
    m_pRenderer(std::move(a_pRenderer)),
    m_sSystemName(std::move(a_sSystemName)) {
}
ParticleSystem::~ParticleSystem(){
  // TODO: Delete attributes? See ownshership
}
ParticleSystem::ParticleSystem(ParticleSystem&& other) 
  :m_pComponent(std::move(other.m_pComponent)),
  m_pRenderer(std::move(other.m_pRenderer)),
  m_sSystemName(std::move(other.m_sSystemName)) {
}
ParticleSystem& ParticleSystem::operator=(ParticleSystem&& other) {
  m_pComponent = std::move(other.m_pComponent);
  m_pRenderer = std::move(other.m_pRenderer);
  m_sSystemName = std::move(other.m_sSystemName);
  return *this;
}
void ParticleSystem::Init() {
  // Set a reference to particles data in the renderer
  m_pRenderer->Init(m_pComponent->GetParticles());
}
void ParticleSystem::Terminate() {
  // Deallocate graphical ressources handled by renderer
  m_pRenderer->Terminate();
}
void ParticleSystem::Update(double a_dt) {
  // Particles update (dynamics, emission/destruction, etc.)
  m_pComponent->Update(a_dt);
  // Rendering buffers update
  m_pRenderer->Update();
}
void ParticleSystem::Render() {
  m_pRenderer->Render();
}
} /* namespace particle */
} /* namespace gem */

