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
#include <memory>
//Other libraries' .h files
#include "glm/glm.hpp"
//Your project's .h files
#include "projects/attractors/proximity_color_updater.hh"
#include "dynamics/particle_attractor.hh"

namespace gem { namespace particle {
namespace attractor_project {
namespace blueprint { namespace attractor_system_builder {
// This creates the said blueprint and adds it to the ParticleModule
void Create();

std::shared_ptr< ParticleAttractor >      GetAttractorHandle();
std::shared_ptr< ProximityColorUpdater >  GetProximityColorUpdaterHandle();

// Some setters for the parameters used for the creation of the system
void SetHotColor(const glm::vec4 &color);
void SetColdColor(const glm::vec4 &color);
void SetPOI(const glm::f32vec3 &pos);
void SetInitialRadius(float radius);
void SetAccelerationRate(float rate);
void SetMaxDistance(float distance);
void SetParticleCount(std::size_t count);
void SetParticleSystemName(const std::string &name);
} /* namespace attractor_system_builder */
} /* namespace blueprint */
} /* namespace attractor_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: ATTRACTOR_SYSTEM_BLUEPRINT_HH */
