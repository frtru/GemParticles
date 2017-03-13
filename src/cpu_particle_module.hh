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
#ifndef CPU_PARTICLE_MODULE_HH
#define CPU_PARTICLE_MODULE_HH

#include "particle_system_interface.hh"

#include <memory>

namespace gem {
namespace particle {
namespace cpu_particle_module {
void Init();
void Terminate();

void Update(double a_dt);

void AddSystem(std::unique_ptr<IParticleSystem> a_pSystem);

// TODOs : See .CC file for todo
void RemoveSystem(const std::string& a_szSystemName); 
void GetSystemByName(const std::string& a_szSystemName);

} /* namespace cpu_particle_module */
} /* namespace particle */
} /* namespace gem */
#endif /* end of include guard: CPU_PARTICLE_MODULE_HH */
