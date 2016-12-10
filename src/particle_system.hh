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

#include "particle_system_component.hh"
#include "renderer.hh"

namespace Gem {
namespace Particle {
namespace ParticleSystem {
void Init();
void Terminate();

void Update(double a_dt);
void Render();

void GetSystemByName(const std::string& a_szSystemName); // See .CC file for todo

void AddSystem(const std::string& a_szSystemName, ParticleSystemComponent&& a_system);
void RemoveSystem(const std::string& a_szSystemName);  // See .CC file for todo

} /* namespace ParticleSystem*/
} /* namespace Particle */
} /* namespace Gem */
#endif /* end of include guard: PARTICLE_SYSTEM_HH */
