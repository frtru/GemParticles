/*************************************************************************
 * Copyright (c) 2016 Fran�ois Trudel
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
#include "projects/attractors/attractor_system_blueprint.hh"
//C system files
//C++ system files
#include <memory>
#include <limits>
//Other libraries' .h files
//Your project's .h files
#include "core/particle_module.hh"
#include "emitters/spherical_stream_emitter.hh"
#include "renderers/core_opengl_renderer.hh"
#include "dynamics/particle_attractor.hh"

//Project specific components
#include "projects/attractors/attractor_particle_system.hh"
#include "projects/attractors/avx_particle_attractor.hh"
#include "projects/attractors/proximity_color_updater.hh"

namespace gem { namespace particle {
namespace attractor_project {
namespace blueprint { namespace attractor_system_builder {
namespace {
const glm::f32vec3 _ZeroVector = glm::f32vec3(0.0f, 0.0f, 0.0f);
// Instead of using setters for every attribute, might as well put them public.
// These parameters will be used during the Create() function to properly build the particle system
glm::u8vec4   _HotColor         = { 255u, 255u, 128u, 255u };
glm::u8vec4   _ColdColor        = { 255u, 0u, 0u, 255u };
glm::f32vec3  _POI              = { 1.0f, 1.0f, 1.0f };
float         _InitialRadius    = 0.5f;
float         _AccelerationRate = 0.5f;
float         _MaxDistance      = 10.0f;
std::size_t   _ParticleCount    = 1000000u;
std::string   _ParticleSystemName;
}

void Create() {
  auto wEmitter = std::make_shared<SphericalStreamEmitter>(_POI, _ZeroVector, _InitialRadius, 0.0f, std::numeric_limits<double>::max());
  auto wParticleSystem = std::make_unique<ParticleSystem<LifeDeathCycle::Disabled> >(_ParticleCount, _ParticleSystemName, wEmitter);
  wParticleSystem->BindRenderer(std::make_unique<CoreGLRenderer>());
  wParticleSystem->AddDynamic(std::make_unique<ParticleAttractor>(_POI, _AccelerationRate));
  wParticleSystem->AddDynamic(std::make_unique<ProximityColorUpdater>(_POI, _HotColor, _ColdColor, _MaxDistance));
  particle_module::AddSystem(std::move(wParticleSystem));
}

void SetHotColor(const glm::u8vec4 &color)          { _HotColor = color;          }
void SetColdColor(const glm::u8vec4 &color)         { _ColdColor = color;         }
void SetPOI(const glm::f32vec3 &pos)                { _POI = pos;                 }
void SetInitialRadius(float radius)                 { _InitialRadius = radius;    }
void SetAccelerationRate(float rate)                { _AccelerationRate = rate;   }
void SetMaxDistance(float distance)                 { _MaxDistance = distance;    }
void SetParticleCount(std::size_t count)            { _ParticleCount = count;     }
void SetParticleSystemName(const std::string &name) { _ParticleSystemName = name; }
} /* namespace attractor_system_builder */
} /* namespace blueprint */
} /* namespace attractor_project */
} /* namespace particle */
} /* namespace gem */

