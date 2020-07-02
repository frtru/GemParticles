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
#include "projects/light/light_project.hh"
//C system files
//C++ system files
#include <memory>
//Other libraries' .h files
#include "utils/imgui/imgui_glfw.h"
#include "utils/imgui/imgui_impl_opengl3.h"
//Your project's .h files
#include "projects/project_dictionary.hh"
#include "utils/timer.hh"
#include "utils/shader_module.hh"
#include "utils/shader_factory.hh"
#include "utils/texture_module.hh"
#include "utils/camera.hh"
#include "utils/imgui/imgui_log.h"
#include "utils/imgui/imgui_property_editor.h"
#include "utils/light_module.hh"
#include "utils/graphic_contexts/opengl_context.hh"
#include "core/particle_module.hh"

// Specific project particle blueprints
#include "projects/light/light_event_handler.hh"
#include "projects/light/scene_with_light.hh"

namespace gem { namespace particle {
namespace light_project {
namespace {
// A pointer to interface, to enable flexibility over
// window management system or 3D API (GLFW/Windows
// & OpenGL/Direct3D)
std::shared_ptr<GraphicContext> graphic_context;
}

REGISTER_STD_PROJECT(light);

void Init() {
  // OpenGL setup
  graphic_context = std::make_shared<OpenGLContext>();
  graphic_context->Init();

  // ImGui initialization
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(graphic_context->GetWindowHandle()), true);
  ImGui_ImplOpenGL3_Init();
  ImGui::StyleColorsClassic();

  shader::module::Init();
  shader::factory::SetShadersFolderBasePath("shaders/light");
  texture::module::Init();

  // Camera initialization
  camera::Init();
  camera::LookAt( 
    glm::vec3(10, 10, 10),   // Camera is at (4,4,4), in World Space
    glm::vec3(0, 0, 0),   // and looks at the origin
    glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)
  camera::SetPerspectiveProjection( 
    glm::radians(45.0f), 
    4.0f, 3.0f, // TODO: This fits the hardcoded 640/480 in the opengl_context.cc file, change this accordingly to changes made in the other file
    0.1f, 100.0f);

  // Light module initialization
  light::module::Init();

  // Scene initialization
  scene::Init();
  scene::SetDebugOption(true);

  // Particle system initialization
  particle_module::Init();

  // Event handler initialization
  event_handler::Init(graphic_context);
}

void Run() {
  glfwSetWindowTitle(static_cast<GLFWwindow*>(
    graphic_context->GetWindowHandle()), "GemParticles");
  while (!graphic_context->PollWindowClosedEvent()) {
    const double dt = timer::Chrono::GetInstance().GetTimeElapsedInSeconds();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Stats");
    ImGui::Text("Application average %.3f ms / frame(%.1f FPS)\n", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Total active particles = %d\n", particle_module::GetActiveParticlesCount());
    ImGui::End();

    scene::Render();
    particle_module::Update(dt);
    event_handler::Update(); // Has to be placed before before clearing depth buffer bit
    light::module::FlushDataToGPU();

    ImGuiPropertyEditor::GetInstance().Draw("Property editor");
    ImGuiLog::GetInstance().Draw("Debugging logs");
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  graphic_context->Terminate();
}
} /* namespace light_project */
} /* namespace particle */
} /* namespace gem */

