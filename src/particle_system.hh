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
#ifndef PARTICLE_SYSTEM_HH
#define PARTICLE_SYSTEM_HH

#include <vector>
#include <memory>

#include "particle_system_interface.hh"
#include "emitter.hh"
#include "dynamic.hh"
#include "euler_particle_updater.hh"

namespace gem {
namespace particle {
template< class RendererType,
          class ParticlePoolType = ParticlePoolCore>
class ParticleSystem : public IParticleSystem{
public:
  explicit ParticleSystem(
	  std::size_t a_unMaxParticleCount,
    std::string &&a_sSystemName = std::move(std::string("DEFAULT_SYS_NAME")))
    : m_pParticlePool(std::make_shared<ParticlePoolType>(a_unMaxParticleCount)),
      m_pRenderer(std::make_unique<RendererType>(m_pParticlePool)),
      m_sSystemName(std::move(a_sSystemName)) {
    // TODO: Remove the following, because when we'll
    // add an updater for the positions, you don't want to
    // update them twice without knowing it

    // Adding an updater following the euler scheme
    m_vDynamics.push_back(std::make_unique<EulerParticleUpdater>());
  }
  virtual ~ParticleSystem() = default;

  ParticleSystem(ParticleSystem<RendererType>&& other)
    : m_pParticlePool(std::move(other.m_pParticlePool)),
      m_pRenderer(std::move(other.m_pRenderer)),
      m_vEmitters(std::move(other.m_vEmitters)),
      m_vDynamics(std::move(other.m_vDynamics)),
      m_sSystemName(std::move(other.m_sSystemName)) {}

  ParticleSystem<RendererType>& 
    operator=(ParticleSystem<RendererType>&& other) {
    m_pParticlePool = std::move(other.m_pParticlePool);
    m_pRenderer = std::move(other.m_pRenderer);
    m_vEmitters = std::move(other.m_vEmitters);
    m_vDynamics = std::move(other.m_vDynamics);
    m_sSystemName = std::move(other.m_sSystemName);
    return *this;
  }

  virtual inline void AddEmitter(std::unique_ptr<Emitter> a_pEmitter) {
	  m_vEmitters.push_back(std::move(a_pEmitter));
  }
  virtual inline void AddDynamic(std::unique_ptr<Dynamic> a_pDynamic) {
	  m_vDynamics.push_back(std::move(a_pDynamic));
  }

  virtual inline void Update(double a_dt) {
    // Particles components update (dynamics, emission/destruction, etc.)
    for (auto& emmiter : m_vEmitters) {
      emmiter->Emit(a_dt, m_pParticlePool);
    }
    for (auto& dynamic : m_vDynamics) {
      dynamic->Update(a_dt, m_pParticlePool);
    }
    // Rendering buffers update
    m_pRenderer->Update(m_pParticlePool);
  }
  virtual inline void Render() {
    m_pRenderer->Render(m_pParticlePool);
  }

private:
  std::string                             m_sSystemName;
  std::shared_ptr<ParticlePoolType>       m_pParticlePool;
  std::unique_ptr<RendererType>           m_pRenderer;
  std::vector<std::unique_ptr<Emitter> >	m_vEmitters;
  std::vector<std::unique_ptr<Dynamic> >	m_vDynamics;
}; /* class ParticleSystem */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: PARTICLE_SYSTEM_HH */
