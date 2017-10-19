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

//Project specific components
#include "projects/attractors/attractor_particle_system.hh"
#include "projects/attractors/avx_particle_attractor.hh"

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
float         _AccelerationRate = 15.0f;
float         _MaxDistance      = 7.0f;
std::size_t   _ParticleCount    = 1000000u;
std::string   _ParticleSystemName;

// Handles on the dynamics to hand them over to the event handler
// There are only used during the construction of the particle system
std::shared_ptr< ParticleAttractor >      _AttractorDynamicHandle;
std::shared_ptr< ProximityColorUpdater >  _ProximityColorUpdaterHandle;
}

void Create() {
  _AttractorDynamicHandle = std::make_shared<ParticleAttractor>(_POI, _AccelerationRate);
  _ProximityColorUpdaterHandle = std::make_shared<ProximityColorUpdater>(_POI, _HotColor, _ColdColor, _MaxDistance);

  auto wEmitter = std::make_shared<SphericalStreamEmitter>(_POI, _ZeroVector, _InitialRadius, 0.0f, std::numeric_limits<double>::max());
  auto wParticleSystem = std::make_unique<ParticleSystem<LifeDeathCycle::Disabled> >(_ParticleCount, _ParticleSystemName, wEmitter);

  wParticleSystem->BindRenderer(std::make_shared<CoreGLRenderer>());
  wParticleSystem->AddDynamic(_AttractorDynamicHandle);
  wParticleSystem->AddDynamic(_ProximityColorUpdaterHandle);

  particle_module::AddSystem(std::move(wParticleSystem));
}

std::shared_ptr< ParticleAttractor >      GetAttractorHandle() { return _AttractorDynamicHandle; }
std::shared_ptr< ProximityColorUpdater >  GetProximityColorUpdaterHandle() { return _ProximityColorUpdaterHandle; }

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

