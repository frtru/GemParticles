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
#include "camera.hh"
#include "particle_system.hh"

// TODO: Temporary includes since test suite
// is not built yet...
#include "particle_system_component.hh"
#include "stub_renderer.hh"
#include "spherical_stream_source.hh"
#include "global_acceleration.hh"

namespace Gem {
namespace Particle {
namespace App {
namespace {
std::unique_ptr<GraphicContext> graphic_context;
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

  // Camera initialization
  Camera::Init();

  // Temporary part, todo move this into a factory or something

  // Particle system initialization
  std::shared_ptr<ParticleSystemComponent> wTempParticleComp = 
    std::make_shared<ParticleSystemComponent>(
      "OBVIOUSLY_TEMPORARY",
      100000);
  wTempParticleComp->AddSource(
    std::make_unique<SphericalStreamSource>(
    SphericalStreamSource()));
  wTempParticleComp->AddDynamic(
    std::make_unique<GlobalAcceleration>()
    );

  std::shared_ptr<Renderer> wTempRenderer = std::make_shared<StubRenderer>();
  ParticleSystem::AddComponents(wTempParticleComp, wTempRenderer);
}

float points[] = {
  0.0f,0.0f,0.0f,
  1.0f,0.0f,0.0f,
  0.0f,0.0f,0.0f,
  0.0f,1.0f,0.0f,
  0.0f,0.0f,0.0f,
  0.0f,0.0f,1.0f
};

void Run() {

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), points, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


  while (!graphic_context->PollWindowClosedEvent()) {
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 2);
    glDrawArrays(GL_LINES, 2, 2);
    glDrawArrays(GL_LINES, 4, 2);
    glBindVertexArray(0);
    //std::cout << "FPS: " << timer::chrono::GetFPS() << std::endl;
    //TODO: See how UI with anttweakbar goes, but
    //events subscription should go here if there's any
    double dt = timer::chrono::GetTimeElapsed<std::chrono::nanoseconds>()
      / timer::NANO_PER_SEC;

    ParticleSystem::Update(dt);

    // TODO: Pre-rendering setup
    // 1- Send camera settings to shader
    glPointSize(3);
    ParticleSystem::Render();
    
    graphic_context->Update();
    timer::chrono::Update();
  }
}

void Terminate() {
  // App destruction
  ParticleSystem::Terminate();
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

