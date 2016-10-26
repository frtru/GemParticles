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
#include "app.hh"
//C system files
//C++ system files
#include <memory>
#include <iostream>
//Other libraries' .h files
//Your project's .h files
#include "timer.hh"
#include "opengl_context.hh"
#include "shader.hh"
#include "particle_system.hh"

// TODO: Temporary includes since test suite
// is not built yet...
#include "fountain_source.hh"
#include "global_acceleration.hh"

namespace Gem {
namespace Particle {
namespace App {
namespace {
std::unique_ptr<GraphicContext> graphic_context;
System particle_system(100000);
//TODO: GPU updater/renderer goes here
}

void Init() {
  // OpenGL setup
  graphic_context = std::make_unique<OpenGLContext>();
  graphic_context->Init();
  
  // Shaders initialization
  ShaderManager::Init();
  ShaderManager::LoadFromFile(GL_VERTEX_SHADER,   "shaders/default.vert");
  ShaderManager::LoadFromFile(GL_FRAGMENT_SHADER, "shaders/default.frag");
  
  ShaderManager::CreateAndLink();
  ShaderManager::Bind();

  // Particle system initialization
  particle_system.AddSource(
    std::make_unique<Gem::Particle::FountainSource>(
      Gem::Particle::FountainSource({ 0.0f,0.0f,0.0f })
      )
  );
  particle_system.AddDynamic(
    std::make_unique<Gem::Particle::GlobalAcceleration>()
  );
}

void Run() {
  while (!graphic_context->PollWindowClosedEvent()) {
    std::cout << "FPS: " << timer::chrono::GetFPS() << std::endl;
    //TODO: See how UI with anttweakbar goes, but
    //events subscription should go here if there's any
    double dt = timer::chrono::GetTimeElapsed<std::chrono::nanoseconds>()
      / timer::NANO_PER_SEC;
    particle_system.Update(dt);

    //TODO: Render here
    //m_particleSystem.Render()

    graphic_context->Update();
    timer::chrono::Update();
  }
  // TODO: Handle destruction of the app properly now since we have a 
  // condition where the window is closed
  ShaderManager::Terminate();
  graphic_context->Terminate();
}

void LoadConfig(const std::string& a_sConfigName) {
  //TODO
}
void SaveConfig(const std::string& a_sConfigName) {
  //TODO
}

} /* namespace App */
} /* namespace Particle */
} /* namespace Gem */

