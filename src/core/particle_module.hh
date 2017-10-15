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
#ifndef PARTICLE_MODULE_HH
#define PARTICLE_MODULE_HH

#include "core/particle_system_interface.hh"

#include <memory>

namespace gem {
namespace particle {
namespace particle_module {
void Init();
void Terminate();

std::size_t GetActiveParticlesCount();

void Update(double a_dt);

void AddSystem(std::unique_ptr<IParticleSystem> a_pSystem);

// TODOs : See .CC file for todo
void RemoveSystem(const std::string& a_sSystemName); 
void GetSystemByName(const std::string& a_sSystemName);

} /* namespace particle_module */
} /* namespace particle */
} /* namespace gem */
#endif /* end of include guard: PARTICLE_MODULE_HH */
