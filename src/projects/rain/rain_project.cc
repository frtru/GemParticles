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
#include "projects/rain/rain_project.hh"
//C system files
//C++ system files
#include <memory>
//Other libraries' .h files
#include "utils/imgui/imgui_glfw.h"
//Your project's .h files
#include "projects/project_dictionary.hh"
#include "utils/timer.hh"
#include "utils/shader_module.hh"
#include "utils/shader_factory.hh"
#include "utils/texture_module.hh"
#include "utils/camera.hh"
#include "utils/scene.hh"
#include "utils/imgui/imgui_log.h"
#include "utils/imgui/imgui_property_editor.h"
#include "utils/basic_event_handler.hh"
#include "utils/graphic_contexts/opengl_context.hh"
#include "core/particle_module.hh"

// Project specific modules
#include "projects/rain/rain_particles_blueprint.hh"

namespace gem { namespace particle {
namespace rain_project {
namespace {
// A pointer to interface, to enable flexibility over
// window management system or 3D API (GLFW/Windows
// & OpenGL/Direct3D)
std::shared_ptr<GraphicContext> graphic_context;
}

REGISTER_STD_PROJECT(rain);

void Init() {
  // OpenGL setup
  graphic_context = std::make_shared<OpenGLContext>();
  graphic_context->Init();

  // ImGui initialization
  ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(graphic_context->GetWindowHandle()), true);
  ImGui::StyleColorsClassic();

  shader::module::Init();
  shader::factory::SetShadersFolderBasePath("src/projects/rain/shaders/");
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
  blueprint::rain_particles_builder::SetParticleSystemName("Amazing rain system");
  blueprint::rain_particles_builder::SetTexture("textures/raindrop.png");
  blueprint::rain_particles_builder::SetEmissionRate(100000.0);
  blueprint::rain_particles_builder::SetParticleCount(1000000u);
  blueprint::rain_particles_builder::Create();
}

void Run() {

  while (!graphic_context->PollWindowClosedEvent()) {
    double dt = timer::Chrono::GetInstance().GetTimeElapsedInSeconds();
    ImGui_ImplGlfw_NewFrame();

    ImGui::Begin("Stats");
    ImGui::Text("Application average %.3f ms / frame(%.1f FPS)\n", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Total active particles = %d\n", particle_module::GetActiveParticlesCount());
    ImGui::End();
    
    scene::Render();
    particle_module::Update(dt);

    ImGuiPropertyEditor::GetInstance().Draw("Property editor");
    ImGuiLog::GetInstance().Draw("Debugging logs");
    ImGui::Render();

    graphic_context->Update();
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
  ImGui_ImplGlfw_Shutdown();
  graphic_context->Terminate();
}
} /* namespace rain_project */
} /* namespace particle */
} /* namespace gem */

