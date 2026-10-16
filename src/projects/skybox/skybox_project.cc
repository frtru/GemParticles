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
#include "projects/skybox/skybox_project.hh"
//C system files
//C++ system files
#include <memory>
//Other libraries' .h files
//Your project's .h files
#include "utils/shader_module.hh"
#include "utils/shader_factory.hh"
#include "utils/texture_module.hh"
#include "utils/camera.hh"
#include "utils/light_module.hh"
#include "utils/graphic_contexts/graphic_context.hh"
#include "core/particle_module.hh"

// Specific project particle blueprints
#include "projects/skybox/skybox_event_handler.hh"
#include "projects/skybox/skybox_scene.hh"

namespace gem { namespace particle {
namespace skybox_project {
void Init(const std::shared_ptr<GraphicContext>& a_pContext) {
  shader::module::Init();
  shader::factory::SetShadersFolderBasePath("shaders/Skybox");
  texture::module::Init();

  // Camera initialization
  camera::Init();
  camera::LookAt( 
    glm::vec3(4, 4, 4),   // Camera is at (4,4,4), in World Space
    glm::vec3(0, 0, 0),   // and looks at the origin
    glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)
  camera::SetPerspectiveProjection( 
    glm::radians(45.0f),
    a_pContext->GetWindowWidth(),
    a_pContext->GetWindowHeight(),
    0.1f, 100.0f);

  // Light module initialization
  light::module::Init();

  // Scene initialization
  scene::Init();
  scene::SetDebugOption(true);

  // Particle system initialization
  particle_module::Init();

  // Event handler initialization
  event_handler::Init(a_pContext);
}

void Update(double a_dt) {
  scene::Render();
  particle_module::Update(a_dt);
  event_handler::Update(); // Has to be placed before before clearing depth buffer bit
  light::module::FlushDataToGPU();
}

void Terminate() {
  // App destruction
  particle_module::Terminate();
  light::module::Terminate();
  scene::Terminate();
  event_handler::Terminate();
  texture::module::Terminate();
  shader::module::Terminate();
}
} /* namespace skybox_project */
} /* namespace particle */
} /* namespace gem */

