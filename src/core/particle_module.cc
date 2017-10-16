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
#include "core/particle_module.hh"

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <mutex>
#include <numeric>

namespace gem {
namespace particle {
namespace particle_module {
namespace {
std::once_flag init_flag;
std::once_flag terminate_flag;
std::vector<std::unique_ptr<IParticleSystem> > m_pSystems;
}

void Init() { std::call_once(init_flag,[&](){}); }
void Terminate() { std::call_once(terminate_flag,[&](){ }); }

void Update(double a_dt) {
  // TODO: This update could very well be 
  // executed in parallel for each systems

  // TODO: Evaluate if the copy of the pair could
  // alter performance. Should we just use the index
  // based loop?
  for (std::size_t i = 0; i < m_pSystems.size(); ++i) {
    m_pSystems[i]->Update(a_dt);
    m_pSystems[i]->Render();
  }
}

IParticleSystem* GetSystemByName(const std::string& a_sSystemName) {
  return std::find_if(m_pSystems.begin(), m_pSystems.end(), 
    [&](const std::unique_ptr<IParticleSystem>& system) {
      return system->GetSystemName() == a_sSystemName;
  })->get();
}

void AddSystem(std::unique_ptr<IParticleSystem> a_pSystem) {
  m_pSystems.push_back(std::move(a_pSystem));
  std::sort(m_pSystems.begin(), m_pSystems.end(), [](
    const std::unique_ptr<IParticleSystem> &l, 
    const std::unique_ptr<IParticleSystem> &r) {
      return l->GetProgramID() < r->GetProgramID();
  });
}

void RemoveSystem(const std::string& a_sSystemName) {
  m_pSystems.erase(
    std::remove_if(m_pSystems.begin(), m_pSystems.end(), 
      [&](const std::unique_ptr<IParticleSystem>& system) {
      return system->GetSystemName() == a_sSystemName;
    }), 
    m_pSystems.end());
}

std::size_t GetActiveParticlesCount() {
  return std::accumulate(m_pSystems.begin(), m_pSystems.end(), static_cast<std::size_t>(0U), 
    [&](std::size_t result, const std::unique_ptr<IParticleSystem>& system) {
      return result + system->GetActiveParticlesCount();
  });
}
} /* namespace particle_module */
} /* namespace particle */
} /* namespace gem */