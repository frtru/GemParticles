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
#include "projects/lit_particles/lit_particles_blueprint.hh"
//C system files
//C++ system files
#include <memory>
#include <limits>
//Other libraries' .h files
//Your project's .h files
#include "core/particle_module.hh"
#include "core/particle_system.hh"
#include "dynamics/gravity_acceleration.hh"
#include "emitters/random_fountain_emitter.hh"

//Project specific components
#include "projects/lit_particles/lit_particles_renderer.hh"
#include "projects/lit_particles/lit_particles_updater.hh"
#include "projects/lit_particles/ground_collision.hh"


namespace gem { namespace particle {
namespace lit_particles_project {
namespace blueprint { namespace lit_particles_system_builder {
namespace {
const glm::f32vec3 _ZeroVector = glm::f32vec3(0.0f, 0.0f, 0.0f);
// Instead of using setters for every attribute, might as well put them public.
// These parameters will be used during the Create() function to properly build the particle system
//glm::u8vec4   _ParticleColor    = { 255u, 0u, 255u, 255u };
glm::u8vec4   _ParticleColor    = { 240u, 186u, 0u, 255u };
glm::f32vec3  _POI              = { 0.0f, 0.0f, -1.5f };
std::size_t   _ParticleCount    = 5001u;
std::string   _TexturePath;
std::string   _ParticleSystemName;

// Handles on the dynamics to hand them over to the event handler
// There are only used during the construction of the particle system
}

void Create() {
  auto wParticleSystem = std::make_unique<ParticleSystem<LifeDeathCycle::Enabled, CoreParticles> >(_ParticleCount, _ParticleSystemName);
  wParticleSystem->BindRenderer(std::make_unique<TextureCoreGLRenderer>(_TexturePath,0.2f));
  wParticleSystem->AddDynamic(std::make_unique<LitParticleUpdater>());
  wParticleSystem->AddDynamic(std::make_unique<GroundCollision>());
  wParticleSystem->AddDynamic(std::make_unique<GravityAcceleration>());
  wParticleSystem->AddEmitter(std::make_unique<RandomFountainEmitter>(_POI, 1.5f, 600.0, 4.0f, _ParticleColor));
  particle_module::AddSystem(std::move(wParticleSystem));
}

void SetTexture(const std::string& a_sTexturePath) { _TexturePath = a_sTexturePath; }
void SetParticleColor(const glm::vec4 &color)       { _ParticleColor = color;     }
void SetPOI(const glm::f32vec3 &pos)                { _POI = pos;                 }
void SetParticleCount(std::size_t count)            { _ParticleCount = count;     }
void SetParticleSystemName(const std::string &name) { _ParticleSystemName = name; }
} /* namespace lit_particles_system_builder */
} /* namespace blueprint */
} /* namespace lit_particles_project */
} /* namespace particle */
} /* namespace gem */

