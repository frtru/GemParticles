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
#include "particle_pool_core.hh"

namespace gem {
namespace particle {
struct LifeDeathCycle {
  struct Disabled {};
  struct Enabled {};
};

template< class RendererType, typename T = LifeDeathCycle::Enabled,
          typename ParticleType = CoreParticles>
class ParticleSystem : public IParticleSystem{
public:

  // SFINAE is used here to describe the lifecycle of particles:
  // particles can either die after a period of time or be
  // be persistent through the whole execution of the program

  template <typename U = T, typename std::enable_if<
    std::is_same<U, LifeDeathCycle::Disabled>::value, int>::type = 0>
  explicit ParticleSystem(std::size_t a_unMaxParticleCount,
    std::string&& a_sSystemName = std::move(std::string("DEFAULT_SYS_NAME")));

  template <typename U = T, typename std::enable_if<
    std::is_same<U, LifeDeathCycle::Enabled>::value, int>::type = 0>
  explicit ParticleSystem(std::size_t a_unMaxParticleCount,
    std::string&& a_sSystemName = std::move(std::string("DEFAULT_SYS_NAME")));

  virtual ~ParticleSystem() = default;

  ParticleSystem(ParticleSystem<RendererType, T>&& other);

  ParticleSystem<RendererType, T>&  operator=(
    ParticleSystem<RendererType, T>&& other);

  std::size_t GetProgramID() const override;
  std::size_t GetActiveParticlesCount() const override;

  void AddEmitter(std::unique_ptr<Emitter<ParticleType>> a_pEmitter);
  void AddDynamic(std::unique_ptr<Dynamic<ParticleType>> a_pDynamic);

  void Update(double a_dt) override;
  void Render() override;

private:
  template <typename U = T, typename std::enable_if<
    std::is_same<U, LifeDeathCycle::Disabled>::value, int>::type = 0>
  void Spawn(double a_dt);

  template <typename U = T, typename std::enable_if<
    std::is_same<U, LifeDeathCycle::Enabled>::value, int>::type = 0>
  void Spawn(double a_dt);

  std::string                                               m_sSystemName;
  std::shared_ptr<ParticlePool<ParticleType> >              m_pParticlePool;
  std::vector<std::unique_ptr<Emitter<ParticleType> > >	    m_vEmitters;
  std::vector<std::unique_ptr<Dynamic<ParticleType> > >	    m_vDynamics;
  std::unique_ptr<RendererType>                             m_pRenderer;
}; /* class ParticleSystem */

template <class RendererType, typename T, typename ParticleType>
template <typename U, typename std::enable_if<std::is_same<U, LifeDeathCycle::Disabled>::value, int>::type>
ParticleSystem<RendererType, T, ParticleType>::ParticleSystem(std::size_t a_unMaxParticleCount, std::string&& a_sSystemName) 
  : m_sSystemName(std::move(a_sSystemName)),
    m_pParticlePool(std::make_shared<ParticlePool<ParticleType>>(a_unMaxParticleCount)),
    m_pRenderer(std::make_unique<RendererType>(m_pParticlePool)) {
  for (std::size_t i = 0; i < m_pParticlePool->GetParticleCount(); ++i) {
    m_pParticlePool->Wake(i);
  }
}

template <class RendererType, typename T, typename ParticleType>
template <typename U, typename std::enable_if<std::is_same<U, LifeDeathCycle::Enabled>::value, int>::type>
ParticleSystem<RendererType, T, ParticleType>::ParticleSystem(std::size_t a_unMaxParticleCount, std::string&& a_sSystemName)
  : m_sSystemName(std::move(a_sSystemName)),
    m_pParticlePool(std::make_shared<ParticlePool<ParticleType>>(a_unMaxParticleCount)),
    m_pRenderer(std::make_unique<RendererType>(m_pParticlePool)) { }

  template <class RendererType, typename T, typename ParticleType>
ParticleSystem<RendererType, T, ParticleType>::ParticleSystem(ParticleSystem<RendererType, T>&& other)
  : m_sSystemName(std::move(other.m_sSystemName)),
    m_pParticlePool(std::move(other.m_pParticlePool)),
    m_vEmitters(std::move(other.m_vEmitters)),
    m_vDynamics(std::move(other.m_vDynamics)),
    m_pRenderer(std::move(other.m_pRenderer)) { }

template <class RendererType, typename T, typename ParticleType>
ParticleSystem<RendererType, T>& ParticleSystem<RendererType, T, ParticleType>::operator=(ParticleSystem<RendererType, T>&& other) {
  m_pParticlePool = std::move(other.m_pParticlePool);
  m_pRenderer = std::move(other.m_pRenderer);
  m_vEmitters = std::move(other.m_vEmitters);
  m_vDynamics = std::move(other.m_vDynamics);
  m_sSystemName = std::move(other.m_sSystemName);
  return *this;
}

template <class RendererType, typename T, typename ParticleType>
std::size_t ParticleSystem<RendererType, T, ParticleType>::GetProgramID() const {
  return m_pRenderer->GetProgramID();
}

template <class RendererType, typename T, typename ParticleType>
std::size_t ParticleSystem<RendererType, T, ParticleType>::GetActiveParticlesCount() const {
  return m_pParticlePool->GetActiveParticleCount();
}

template <class RendererType, typename T, typename ParticleType>
void ParticleSystem<RendererType, T, ParticleType>::AddEmitter(std::unique_ptr<Emitter<ParticleType>> a_pEmitter) {
  m_vEmitters.push_back(std::move(a_pEmitter));
}

template <class RendererType, typename T, typename ParticleType>
void ParticleSystem<RendererType, T, ParticleType>::AddDynamic(std::unique_ptr<Dynamic<ParticleType>> a_pDynamic) {
  m_vDynamics.push_back(std::move(a_pDynamic));
}

template <class RendererType, typename T, typename ParticleType>
void ParticleSystem<RendererType, T, ParticleType>::Update(double a_dt) {
  // Particle initializations/creation
  Spawn(a_dt);
  // Dynamics/runtime behavior update
  for (auto& dynamic : m_vDynamics) {
    dynamic->Update(a_dt, m_pParticlePool);
  }
  // Rendering buffers update
  m_pRenderer->Update(m_pParticlePool);
}

template <class RendererType, typename T, typename ParticleType>
void ParticleSystem<RendererType, T, ParticleType>::Render() {
  m_pRenderer->Render(m_pParticlePool);
}

template <class RendererType, typename T, typename ParticleType>
template <typename U, typename std::enable_if<std::is_same<U, LifeDeathCycle::Disabled>::value, int>::type>
void ParticleSystem<RendererType, T, ParticleType>::Spawn(double a_dt) { }

template <class RendererType, typename T, typename ParticleType>
template <typename U, typename std::enable_if<std::is_same<U, LifeDeathCycle::Enabled>::value, int>::type>
void ParticleSystem<RendererType, T, ParticleType>::Spawn(double a_dt) {
  for (auto& emmiter : m_vEmitters) {
    emmiter->Emit(a_dt, m_pParticlePool);
  }
}
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: PARTICLE_SYSTEM_HH */
