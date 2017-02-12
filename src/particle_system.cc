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

namespace Gem {
namespace Particle {
ParticleSystem::ParticleSystem(
  const std::shared_ptr<ParticleSystemComponent> &a_pComponent,
  const std::shared_ptr<Renderer> &a_pRenderer)
  : m_pComponent(a_pComponent),
    m_pRenderer(a_pRenderer) {
}
ParticleSystem::~ParticleSystem(){
  // TODO: Delete attributes? See ownshership
}
void ParticleSystem::Init() {
  // Set a reference to particles data in the renderer
  m_pRenderer->Init(m_pComponent->GetParticles().get());
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
} /* namespace Particle */
} /* namespace Gem */

