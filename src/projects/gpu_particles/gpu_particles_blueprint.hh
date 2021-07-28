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
#ifndef GPU_PARTICLES_BLUEPRINT_HH
#define GPU_PARTICLES_BLUEPRINT_HH

//C system files
//C++ system files
#include <string>
#include <memory>
//Other libraries' .h files
#include "glm/glm.hpp"
//Your project's .h files

namespace gem { namespace particle {
namespace gpu_particles_project {
namespace blueprint { namespace gpu_particles_system_builder {
// This creates the said blueprint and adds it to the ParticleModule
void Create();
// Some setters for the parameters used for the creation of the system
void SetParticleColor(const glm::vec4 &color);
void SetPOI(const glm::f32vec3 &pos);
void SetParticleCount(std::size_t count);
void SetTexture(const std::string& a_sTexturePath);
void SetParticleSystemName(const std::string &name);
} /* namespace gpu_particles_system_builder */
} /* namespace blueprint */
} /* namespace gpu_particles_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: LIT_PARTICLES_BLUEPRINT_HH */
