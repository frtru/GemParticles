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
#ifndef ATTRACTOR_PARTICLE_SYSTEM_HH
#define ATTRACTOR_PARTICLE_SYSTEM_HH

#include <vector>
#include <memory>
#include <iostream>

#include "core/particle_system_interface.hh"
#include "core/particle_pool_core.hh"
#include "emitters/emitter.hh"
#include "dynamics/dynamic.hh"
#include "renderers/renderer.hh"

namespace gem { namespace particle {
namespace attractor_project {
struct LifeDeathCycle {
  struct Disabled {};
  struct Enabled {};
};
//
// Class template specialization for T=LifeDeathCycle::Disabled
//
// We redefine the constructor to wake all the particles during
// initialization, instead of doing it at each frame.
// We also redefine the spawn method so that it is empty and 
// make sure there's no 
//
template< typename T = LifeDeathCycle::Disabled,
          typename ParticleType = CoreParticles>
class ParticleSystem : public IParticleSystem {
public:
  explicit ParticleSystem(std::size_t a_unMaxParticleCount, const std::string& a_sSystemName,
    std::shared_ptr<Emitter<ParticleType> > a_pEmitter)
    : m_sSystemName(std::move(a_sSystemName)), m_pRenderer(nullptr),
    m_pParticlePool(std::make_shared<ParticlePool<ParticleType>>(a_unMaxParticleCount)) {
    a_pEmitter->Emit(1.0f,m_pParticlePool);
  }
  virtual ~ParticleSystem() = default;

  std::size_t GetProgramID() const override { return m_pRenderer->GetProgramID(); }
  std::size_t GetActiveParticlesCount() const override { return m_pParticlePool->GetActiveParticleCount(); }

  void AddDynamic(std::unique_ptr<Dynamic<ParticleType>> a_pDynamic) {
    if (a_pDynamic->AltersParticleLifeCycle()) {
      std::cout << "ERROR: ParticleSystem::AddDynamic -> Tried to add a ";
      std::cout << "dynamic which alters the Life-Death cycle when it is disabled. ";
      std::cout << "Dynamic will not be added to the system." << std::endl;
      return;
    }
    m_vDynamics.push_back(std::move(a_pDynamic));
  }
  void BindRenderer(std::unique_ptr<Renderer<ParticleType> > a_pRenderer) {
    m_pRenderer = std::move(a_pRenderer);
    m_pRenderer->Bind(m_pParticlePool);
  }

  void Update(double a_dt) override {
    // Dynamics/runtime behavior update
    for (auto& dynamic : m_vDynamics) {
      dynamic->Update(a_dt, m_pParticlePool);
    }
    // Rendering buffers update
    m_pRenderer->Update();
  }
  void Render() override {
    m_pRenderer->Render();
  }

private:
  std::string                                               m_sSystemName;
  std::shared_ptr<ParticlePool<ParticleType> >              m_pParticlePool;
  std::vector<std::unique_ptr<Dynamic<ParticleType> > >	    m_vDynamics;
  std::unique_ptr<Renderer<ParticleType> >                  m_pRenderer;
}; /* class ParticleSystem<LifeDeathCycle::Disabled, ParticleType> */
} /* attractor_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: ATTRACTOR_PARTICLE_SYSTEM_HH */
