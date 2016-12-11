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

#include <vector>
#include <map>
#include <string>
#include <memory>

namespace Gem {
namespace Particle {
namespace ParticleSystem {
namespace {
// Some helper accessors and redefinitions for readability
using pParticleComponent  = std::shared_ptr<ParticleSystemComponent>;
using pRenderingComponent = std::shared_ptr<Renderer>;
using Components          = std::pair<pParticleComponent, pRenderingComponent >;
using ComponentsList      = std::vector<Components>;

ComponentsList sub_systems;

pParticleComponent&   GetParticleComponent(Components& rhs)  { return rhs.first; }
pRenderingComponent&  GetRenderingComponent(Components& rhs) { return rhs.second; }

pParticleComponent&   GetParticleComponent(std::size_t rhs)  { return sub_systems[rhs].first; }
pRenderingComponent&  GetRenderingComponent(std::size_t rhs) { return sub_systems[rhs].second; }
}

void Init() {
}

void Terminate() {
  for (auto sub_system : sub_systems) {
    GetRenderingComponent(sub_system)->Terminate();
  }
}

void Update(double a_dt) {
  // TODO: This update could very well be 
  // executed in parallel for each systems

  // TODO: Evaluate if the copy of the pair could
  // alter performance. Should we just use the index
  // based loop?
  for (auto sub_system : sub_systems) {
    // Particles update (dynamics, emission/destruction, etc.)
    GetParticleComponent(sub_system)->Update(a_dt);

    // Rendering buffers update
    GetRenderingComponent(sub_system)->Update();
  }
}

void Render() {
  for (auto sub_system : sub_systems) {
    GetRenderingComponent(sub_system)->Render();
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

void AddComponents(
  const std::shared_ptr<ParticleSystemComponent>& a_pParticleComponent, 
  const std::shared_ptr<Renderer>& a_pRenderer) {
  a_pRenderer->Init(a_pParticleComponent->GetParticles().get());
  sub_systems.push_back({ a_pParticleComponent, a_pRenderer });
}

void RemoveSystem(const std::string& a_szSystemName) {
  /*
  TODO:
    This could be necessary, i can see a use case, but this is not priority
    RemoveByName maybe
  */
}
} /* namespace ParticleSystem */
} /* namespace Particle */
} /* namespace Gem */