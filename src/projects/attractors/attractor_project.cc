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
#include "projects/attractors/attractor_project.hh"
//C system files
//C++ system files
#include <memory>
#include <iostream>
//Other libraries' .h files
#include "utils/imgui/imgui_glfw.h"
//#include <AntTweakBar.h>
//Your project's .h files
#include "project_dictionary.hh"
#include "utils/timer.hh"
#include "utils/shader_module.hh"
#include "utils/shader_factory.hh"
#include "utils/texture_module.hh"
#include "utils/camera.hh"
#include "utils/scene.hh"
#include "graphic_contexts/opengl_context.hh"
#include "core/particle_module.hh"

// Specific project particle blueprints
#include "projects/attractors/attractor_event_handler.hh"
#include "projects/attractors/attractor_system_blueprint.hh"

namespace gem { namespace particle {
namespace attractor_project {
namespace {
// A pointer to interface, to enable flexibility over
// window management system or 3D API (GLFW/Windows
// & OpenGL/Direct3D)
std::shared_ptr<GraphicContext> graphic_context;
}

void RegisterProject() {
  project_dict::AddStage("attractor", std::bind(&Init));
  project_dict::AddStage("attractor", std::bind(&Run));
  project_dict::AddStage("attractor", std::bind(&Terminate));
}

void Init() {
  // OpenGL setup
  graphic_context = std::make_shared<OpenGLContext>();
  graphic_context->Init();

  shader::module::Init();
  shader::factory::SetShadersFolderBasePath("shaders/");
  texture::module::Init();

  // ImGui initialization
  ImGui_ImplGlfwGL3_Init(static_cast<GLFWwindow*>(graphic_context->GetWindowHandle()), true);
  ImGui::StyleColorsClassic(); 

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
  
  // Scene initialization
  scene::Init();
  scene::SetDebugOption(true);

  // Particle system initialization
  particle_module::Init();
  blueprint::attractor_system_builder::SetParticleSystemName("Amazing attractor system");
  blueprint::attractor_system_builder::SetParticleCount(500001u);
  blueprint::attractor_system_builder::Create();

  // Event handler initialization
  event_handler::Init(graphic_context, 
    blueprint::attractor_system_builder::GetAttractorHandle(),
    blueprint::attractor_system_builder::GetProximityColorUpdaterHandle());
}

void Run() {
  glfwSetWindowTitle(static_cast<GLFWwindow*>(
    graphic_context->GetWindowHandle()), "GemParticles");
  while (!graphic_context->PollWindowClosedEvent()) {
    const double dt = timer::Chrono::GetInstance().GetTimeElapsedInSeconds();
    ImGui_ImplGlfwGL3_NewFrame();

    ImGui::Text("Hello, world!");                           // Some text (you can use a format string too)

    scene::Render();
    particle_module::Update(dt);

    event_handler::Update(); // Has to be placed before before clearing depth buffer bit
    graphic_context->Update();
    ImGui::Render();

    timer::Chrono::GetInstance().Update();
  }
}

void Terminate() {
  // App destruction
  particle_module::Terminate();
  scene::Terminate();
  event_handler::Terminate();
  texture::module::Terminate();
  shader::module::Terminate();
  ImGui_ImplGlfwGL3_Shutdown();
  graphic_context->Terminate();
}
} /* namespace rain_project */
} /* namespace particle */
} /* namespace gem */

