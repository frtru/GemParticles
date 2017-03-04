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
#include "event_handler.hh"
#include "cpu_particle_module.hh"

// TODO: Temporary includes since test suite
// or factory/builder are not built yet...
#include "particle_system_component.hh"
#include "simple_opengl_renderer.hh"
#include "rain_emitter.hh"
#include "gravity_acceleration.hh"

namespace gem {
namespace particle {
namespace app {
namespace {
// A pointer to interface, to enable flexibility over
// window management system or 3D API (GLFW/Windows
// & OpenGL/Direct3D)
std::shared_ptr<GraphicContext> graphic_context;
}

void Init() {
  // OpenGL setup
  graphic_context = std::make_shared<OpenGLContext>();
  graphic_context->Init();

  // Shaders initialization
  ShaderManager::Init();
  ShaderManager::LoadFromFile(GL_VERTEX_SHADER,   "../shaders/default.vert");
  ShaderManager::LoadFromFile(GL_FRAGMENT_SHADER, "../shaders/default.frag");
  
  shader_manager::CreateAndLink();
  shader_manager::Bind();

  // Camera initialization
  camera::Init();
  camera::LookAt( 
    glm::vec3(0, 0, 5),   // Camera is at (0,0,5), in World Space
    glm::vec3(0, 0, 0),   // and looks at the origin
    glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)
  camera::SetPerspectiveProjection( 
    glm::radians(45.0f), 
    4.0f, 3.0f, // TODO: This fits the hardcoded 640/480 in the opengl_context.cc file, change this accordingly to changes made in the other file
    0.1f, 100.0f);

  // Event handler initialization
  event_handler::Init(graphic_context);

  // Particle system initialization
  cpu_particle_module::Init();
  ParticleSystem wParticleSystem(1000000,
	  std::make_unique<SimpleGLRenderer>(),
	  "OBVIOUSLY_TEMPORARY"
  );
  wParticleSystem.AddDynamic(std::make_unique<GravityAcceleration>());
  wParticleSystem.AddEmitter(std::make_unique<RainEmitter>(10.0f, 100000));
  cpu_particle_module::AddSystem(std::move(wParticleSystem));
}

// TODO: Add that as debugging option in one of the renderers maybe?
// Definitely not in the renderers, since it would be duplicated
// in every renderers
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
    std::cout << "FPS: " << timer::chrono::GetFPS() << std::endl;
    //TODO: See how UI with anttweakbar goes, but
    //events subscription should go here if there's any
    double dt = timer::chrono::GetTimeElapsedInSeconds();

    cpu_particle_module::Update(dt);    
    graphic_context->Update();
    timer::chrono::Update();
  }
}

void Terminate() {
  // App destruction
  cpu_particle_module::Terminate();
  shader_manager::Terminate();
  graphic_context->Terminate();
}

void LoadConfig(const std::string& a_sConfigName) {
  //TODO
}
void SaveConfig(const std::string& a_sConfigName) {
  //TODO
}

} /* namespace app */
} /* namespace particle */
} /* namespace gem */

