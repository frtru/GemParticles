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
void Create() {
  auto wEmitter = std::make_shared<SphericalStreamEmitter>(_POI, _ZeroVector, _InitialRadius, 0.0f, std::numeric_limits<double>::max());
  auto wParticleSystem = std::make_unique<ParticleSystem<LifeDeathCycle::Disabled> >(_ParticleCount, _ParticleSystemName, wEmitter);
  wParticleSystem->BindRenderer(std::make_unique<CoreGLRenderer>());
  wParticleSystem->AddDynamic(std::make_unique<ParticleAttractor>(_POI, _AccelerationRate));
  wParticleSystem->AddDynamic(std::make_unique<ProximityColorUpdater>(_POI, _HotColor, _ColdColor, _MaxDistance));
  particle_module::AddSystem(std::move(wParticleSystem));
}
} /* namespace attractor_system_builder */
} /* namespace blueprint */
} /* namespace attractor_project */
} /* namespace particle */
} /* namespace gem */

