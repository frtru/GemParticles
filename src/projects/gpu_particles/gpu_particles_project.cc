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
#include "projects/gpu_particles/gpu_particles_project.hh"
//C system files
//C++ system files
#include <memory>
//Other libraries' .h files
//Your project's .h files
#include "utils/timer.hh"
#include "utils/shader_module.hh"
#include "utils/shader_factory.hh"
#include "utils/texture_module.hh"
#include "utils/camera.hh"
#include "utils/basic_event_handler.hh"
#include "utils/imgui/imgui_log.h"
#include "utils/imgui/imgui_property_editor.h"
#include "utils/graphic_contexts/opengl_context.hh"
#include "core/particle_module.hh"

// Specific project particle blueprints
#include "projects/gpu_particles/gpu_particles_blueprint.hh"
#include "projects/gpu_particles/gpu_particles_scene.hh"

namespace gem { namespace particle {
namespace gpu_particles_project {
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

  shader::module::Init();
  shader::factory::SetShadersFolderBasePath("shaders/GPUParticles");

  // Camera initialization
  camera::Init();
  camera::LookAt( 
    glm::vec3(8, 8, 8),   // Camera is at (8,8,8), in World Space
    glm::vec3(0.0f, 0.0f, 0.0f),   // and looks at the origin
    glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)
  camera::SetPerspectiveProjection( 
    glm::radians(45.0f),
    graphic_context->GetWindowWidth(),
    graphic_context->GetWindowHeight(),
    0.1f, 100.0f);

  // Scene initialization
  scene::Init();
  scene::SetDebugOption(true);

  // Particle system initialization
  particle_module::Init();
  blueprint::gpu_particles_system_builder::SetParticleSystemName("wow");
  blueprint::gpu_particles_system_builder::SetParticleCount(1000001u);
  blueprint::gpu_particles_system_builder::Create();

  // Event handler initialization
  // event_handler::Init(graphic_context);
}

void Run() {
  while (!graphic_context->PollWindowClosedEvent()) {
    const double dt = timer::Chrono::GetInstance().GetTimeElapsedInSeconds();

    graphic_context->NewFrame();

    ImGui::Begin("Stats");
    ImGui::Text("Application average %.3f ms / frame(%.1f FPS)\n", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Total active particles = %d\n", particle_module::GetActiveParticlesCount());
    ImGui::End();

    scene::Render();
    particle_module::Update(dt);

    //TODO: Add custom handler or implement this in base one
    //event_handler::Update(); // Has to be placed before before clearing depth buffer bit

    light::module::FlushDataToGPU();

    ImGuiPropertyEditor::GetInstance().Draw("Property editor");
    ImGuiLog::GetInstance().Draw("Debugging logs");

    graphic_context->Update();
    timer::Chrono::GetInstance().Update();
  }
}

void Terminate() {
  // App destruction
  particle_module::Terminate();
  light::module::Terminate();
  scene::Terminate();
  event_handler::Terminate();
  texture::module::Terminate();
  shader::module::Terminate();
  graphic_context->Terminate();
}
} /* namespace lit particles_project */
} /* namespace particle */
} /* namespace gem */

