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
#ifndef ATTRACTOR_SYSTEM_BLUEPRINT_HH
#define ATTRACTOR_SYSTEM_BLUEPRINT_HH

//C system files
//C++ system files
#include <string>
//Other libraries' .h files
#include "glm/glm.hpp"
//Your project's .h files

namespace gem { namespace particle {
namespace attractor_project {
namespace blueprint { namespace attractor_system_builder {
namespace {
// Instead of using setters for every attribute, might as well put them public.
// These parameters will be used during the Create() function to properly build the particle system
glm::u8vec4   _HotColor         = { 255u, 255u, 128u, 255u };
glm::u8vec4   _ColdColor        = { 255u, 0u, 0u, 255u };
glm::f32vec3  _POI              = { 2.0f, 2.0f, 2.0f };
float         _AccelerationRate = 0.5f;
float         _MaxDistance      = 10.0f;
std::size_t   _ParticleCount    = 1000000u;
std::string   _ParticleSystemName;
}

// This creates the said blueprint and adds it to the ParticleModule
void Create();
} /* namespace attractor_system_builder */
} /* namespace blueprint */
} /* namespace attractor_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: ATTRACTOR_SYSTEM_BLUEPRINT_HH */
