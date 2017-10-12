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
#include "rain_particles_blueprint.hh"
//C system files
//C++ system files
#include <memory>
//Other libraries' .h files
//Your project's .h files
#include "particle_module.hh"
#include "particle_system.hh"
#include "rain_emitter.hh"
#include "gravity_acceleration.hh"
#include "euler_particle_updater.hh"
#include "texture_core_gl_renderer.hh"

namespace gem { namespace particle {
namespace blueprint {
namespace rain_particles_builder {
namespace {
  std::string _TexturePath;
  std::string _ParticleSystemName;
  double      _EmissionRate;
}
void Create() {
  const std::size_t unParticleCount = 1000000u;
  auto wParticleSystem = std::make_unique<ParticleSystem<> >(unParticleCount, _ParticleSystemName);
  wParticleSystem->BindRenderer(std::make_unique<TextureCoreGLRenderer>(_TexturePath));
  wParticleSystem->AddDynamic(std::make_unique<EulerParticleUpdater>());
  wParticleSystem->AddDynamic(std::make_unique<GravityAcceleration>());
  wParticleSystem->AddEmitter(std::make_unique<RainEmitter>(10.0f, _EmissionRate));
  particle_module::AddSystem(std::move(wParticleSystem));
}

void SetTexture(const std::string& a_sTexturePath) { _TexturePath = a_sTexturePath; }
void SetParticleSystemName(const std::string& a_sSystemName) { _ParticleSystemName = a_sSystemName; }
void SetEmissionRate(double a_dEmissionRate) { _EmissionRate = a_dEmissionRate; }
} /* namespace rain_particles_builder */
} /* namespace blueprint */
} /* namespace particle */
} /* namespace gem */
