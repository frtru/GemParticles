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
#include "particle_module.hh"

#include <vector>
#include <map>
#include <string>

namespace gem {
namespace particle {
namespace particle_module {
namespace {
std::vector<ParticleSystem> *m_pSystems;
}

void Init() 
{
  static bool s_bIsInitialized = false;
  if (!s_bIsInitialized) {
    m_pSystems = new std::vector<ParticleSystem>();
    s_bIsInitialized = true;
  }
}

void Terminate() 
{
  static bool s_bIsTerminated = false;
  if (!s_bIsTerminated) {
    for (std::size_t i = 0; i < m_pSystems->size(); ++i) {
      m_pSystems->at(i).Terminate();
    }
    delete m_pSystems;
    s_bIsTerminated = true;
  }
}

void Update(double a_dt) {
  // TODO: This update could very well be 
  // executed in parallel for each systems

  // TODO: Evaluate if the copy of the pair could
  // alter performance. Should we just use the index
  // based loop?
  for (std::size_t i = 0; i < m_pSystems->size(); ++i) {
    m_pSystems->at(i).Update(a_dt);
    m_pSystems->at(i).Render();
  }
}

void GetSystemByName(const std::string& a_szSystemName) {
  /*
   TODO:
   Add accessors in the particlesystemcompoenent for the name
   change the parameeters of this function
   Do two function GetParticleComponentByName and GetRenderingComponentByName
      and adjust the return type with the corresponding funtion name
  */
}

void AddSystem(ParticleSystem &&a_rSystem) {
  a_rSystem.Init();
  m_pSystems->push_back(std::move(a_rSystem));
}

/*void AddComponents(
  const std::shared_ptr<ParticleSystemComponent> &a_pComponent,
  const std::shared_ptr<Renderer> &a_pRenderer) {
  a_pRenderer->Init(a_pComponent->GetParticles().get());
  m_pSystems->push_back(a_pComponent, a_pRenderer});
}*/

void RemoveSystem(const std::string& a_szSystemName) {
  /*
  TODO:
    This could be necessary, i can see a use case, but this is not priority
    RemoveByName maybe
  */
}
} /* namespace ParticleSystem */
} /* namespace particle */
} /* namespace gem */