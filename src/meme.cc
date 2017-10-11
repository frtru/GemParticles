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
#include "meme.hh"
//C system files
//C++ system files
#include <memory>
#include <iostream>
#include <sstream>
//Other libraries' .h files
//Your project's .h files
#include "project_dictionary.hh"
#include "timer.hh"
#include "opengl_context.hh"
#include "shader_module.hh"
#include "texture_module.hh"
#include "camera.hh"
#include "event_handler.hh"
#include "particle_module.hh"
#include "scene.hh"

// Specific project particle blueprints
#include "rain_particles_blueprint.hh"

namespace gem {
namespace particle {
namespace meme {
namespace {
// A pointer to interface, to enable flexibility over
// window management system or 3D API (GLFW/Windows
// & OpenGL/Direct3D)
std::shared_ptr<GraphicContext> graphic_context;
}

void RegisterProject() {
  project_dict::AddStage("meme", std::bind(&Init));
  project_dict::AddStage("meme", std::bind(&Run));
  project_dict::AddStage("meme", std::bind(&Terminate));
}

void Init() {
  // OpenGL setup
  graphic_context = std::make_shared<OpenGLContext>();
  graphic_context->Init();

  shader::module::Init();
  texture::module::Init();

  // Camera initialization
  camera::Init();
  camera::LookAt( 
    glm::vec3(4, 4, 4),   // Camera is at (0,0,4), in World Space
    glm::vec3(0, 0, 0),   // and looks at the origin
    glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)
  camera::SetPerspectiveProjection( 
    glm::radians(45.0f), 
    4.0f, 3.0f, // TODO: This fits the hardcoded 640/480 in the opengl_context.cc file, change this accordingly to changes made in the other file
    0.1f, 100.0f);

  // Event handler initialization
  event_handler::Init(graphic_context);

  // Scene initialization
  scene::Init();
  scene::SetDebugOption(true);

  // Particle system initialization
  particle_module::Init();
  blueprint::rain_particles_builder::Create();
}

void Run() {

  while (!graphic_context->PollWindowClosedEvent()) {
    double dt = timer::chrono::GetTimeElapsedInSeconds();
    std::stringstream ss; 
    ss << "GemParticles, FPS: "  << timer::chrono::GetFPS()
      << " | Active Particles: " << particle_module::GetActiveParticlesCount();
    glfwSetWindowTitle(static_cast<GLFWwindow*>(
      graphic_context->GetWindowHandle()), ss.str().c_str());
    
    scene::Render();
    particle_module::Update(dt);    

    graphic_context->Update();
    timer::chrono::Update();
  }
}

void Terminate() {
  // App destruction
  particle_module::Terminate();
  scene::Terminate();
  event_handler::Terminate();
  texture::module::Terminate();
  shader::module::Terminate();
  graphic_context->Terminate();
}
} /* namespace meme */
} /* namespace particle */
} /* namespace gem */

