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

namespace Gem {
namespace Particle {
namespace ParticleSystem {
namespace {
  //TODO : Use pointers instead of raw entities!!

// See if map is better here
using ComponentsList  = std::vector<std::pair<ParticleSystemComponent, Renderer> >;
using Components      = std::pair<ParticleSystemComponent, Renderer>;

ComponentsList sub_systems;

// Some helper accessors for readability
ParticleSystemComponent& GetParticleComponent(Components& rhs)  { return rhs.first; }
Renderer& GetRenderingComponent(Components& rhs)                { return rhs.second; }

ParticleSystemComponent& GetParticleComponent(std::size_t rhs)  { return sub_systems[rhs].first; }
Renderer& GetRenderingComponent(std::size_t rhs)                { return sub_systems[rhs].second; }
}

void Init() {
}

void Terminate() {
}

void Update(double a_dt) {
  // TODO: This update could very well be 
  // executed in parallel for each systems

  // Update each systems individually
  for (std::size_t i = 0; i < sub_systems.size(); ++i) {
    // TODO: Change this kind of iteration when the change toward pointers will be done
    GetParticleComponent(i).Update(a_dt);
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

void AddComponents(ParticleSystemComponent a_particleComponent, Renderer* a_renderer) {
  sub_systems.push_back(
    std::make_pair<ParticleSystemComponent,Renderer*>(
      a_particleComponent, a_renderer));
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