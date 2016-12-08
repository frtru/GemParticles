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

#include <map>
#include <string>

namespace Gem {
namespace Particle {
namespace ParticleModule {
namespace {
  std::map<std::string, System> particle_systems;
}

void Init() {
}

void Terminate() {
}

void Update(double a_dt) {
  // TODO: This update could very well be 
  // executed in parallel for each systems

  // Update each systems individually
  for (auto system : particle_systems) {
    system.second.Update(a_dt);
  }
}

void GetSystemByName(const std::string& a_szSystemName) {
  particle_systems.at(a_szSystemName);
}

void AddSystem(const std::string& a_szSystemName, System&& a_system) {
  particle_systems.insert({ a_szSystemName, a_system});
}

void RemoveSystem(const std::string& a_szSystemName) {
  particle_systems.erase(a_szSystemName);
}

} /* namespace ParticleModule */
} /* namespace Particle */
} /* namespace Gem */