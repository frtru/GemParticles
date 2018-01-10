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
#include "projects/glass/glass_scene.hh"

#include <iostream>
#include <mutex>

#include <GL/glew.h>

#include "utils/shader_factory.hh"
#include "utils/shader_module.hh"
#include "utils/light_module.hh"
#include "utils/skybox.hh"

namespace gem { namespace particle {
namespace glass_project {
namespace scene {
namespace {
const GLfloat AXES_POINTS[] = {
  0.0f,0.0f,0.0f,
  3.0f,0.0f,0.0f,
  0.0f,0.0f,0.0f,
  0.0f,3.0f,0.0f,
  0.0f,0.0f,0.0f,
  0.0f,0.0f,3.0f
};

const unsigned char AXES_COLOR[] = {
  255u,255u,255u,255u,
  255u,0u,0u,255u,
  255u,255u,255u,255u,
  0u,255u,0u,255u,
  255u,255u,255u,255u,
  0u,0u,255u,255u
};

// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
const GLfloat BOX_POINTS[] = {
  -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,

  -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
  -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
  -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
  -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
  -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
  -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
  1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
  1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

  -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
  -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
  -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,

  -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
  -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
  -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f
};

// NOTE : With the material being sent through with an uniform,
// This is actually not being used. The color is sent in the material directly.
// According to : https://www.opengl.org/discussion_boards/showthread.php/171379-VBOs-Drawing-vertices-of-same-color
// "You have to specify the color for each vertex."
const unsigned char BOX_COLOR[] = {
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
  225u,128u,15u,255u,
};

bool    debug_mode;
GLuint  vertex_array_IDs[2];

const std::size_t POSITIONS_VBO_IDX = 0;
const std::size_t COLORS_VBO_IDX = 1;
//const std::size_t NORMALS_VBO_IDX = 2;

GLuint  axes_VBO_IDs[2];
GLuint  box_VBO_IDs[2];
GLuint  shader_program_ID;

std::once_flag init_flag;
std::once_flag terminate_flag;

light::Material box_material;

void DrawDebugObjects() {
  glBindVertexArray(vertex_array_IDs[0]);
  glDrawArrays(GL_LINES, 0, 2);
  glDrawArrays(GL_LINES, 2, 2);
  glDrawArrays(GL_LINES, 4, 2);
  glBindVertexArray(vertex_array_IDs[1]);
  glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
  glBindVertexArray(0);
}

void InitializeAxes() {
  glGenBuffers(2, axes_VBO_IDs);
  std::cout << "Scene::Init -> Generated axes VBOs, IDs = ";
  std::cout << axes_VBO_IDs[POSITIONS_VBO_IDX] << " & ";
  std::cout << axes_VBO_IDs[COLORS_VBO_IDX] << std::endl;

  // Positions
  glBindBuffer(GL_ARRAY_BUFFER, axes_VBO_IDs[POSITIONS_VBO_IDX]);
  std::cout << "Scene::Init -> Allocated buffer memory for ID = ";
  std::cout << axes_VBO_IDs[POSITIONS_VBO_IDX] << std::endl;
  glBufferData(GL_ARRAY_BUFFER, sizeof(AXES_POINTS), AXES_POINTS, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);

  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(0, axes_VBO_IDs[POSITIONS_VBO_IDX], 0, 3 * sizeof(GL_FLOAT));
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
  }
  else {
    glVertexAttribPointer(
      0, 3,
      GL_FLOAT, GL_FALSE,
      3 * sizeof(GL_FLOAT), nullptr);
  }

  // Colors

  //Color VBO Initialization
  glBindBuffer(GL_ARRAY_BUFFER, axes_VBO_IDs[COLORS_VBO_IDX]);
  std::cout << "Scene::Init -> Allocated buffer memory for ID = ";
  std::cout << axes_VBO_IDs[COLORS_VBO_IDX] << std::endl;
  glBufferData(GL_ARRAY_BUFFER, sizeof(AXES_COLOR), AXES_COLOR, GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);

  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(1, axes_VBO_IDs[COLORS_VBO_IDX], 0, 4 * sizeof(unsigned char));
    glVertexAttribFormat(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0);
    glVertexAttribBinding(1, 1);
  }
  else {
    glVertexAttribPointer(
      1, 4,
      GL_UNSIGNED_BYTE, GL_TRUE,
      4 * sizeof(unsigned char), nullptr);
  }
}
void InitializeTestingBox() {
  glGenBuffers(2, box_VBO_IDs);
  std::cout << "Scene::Init -> Generated 2 VBOs, IDs = ";
  std::cout << box_VBO_IDs[POSITIONS_VBO_IDX] << " & ";
  std::cout << box_VBO_IDs[COLORS_VBO_IDX] << std::endl;

  // Positions
  glBindBuffer(GL_ARRAY_BUFFER, box_VBO_IDs[POSITIONS_VBO_IDX]);
  std::cout << "Scene::Init -> Allocated buffer memory for ID = ";
  std::cout << box_VBO_IDs[POSITIONS_VBO_IDX] << std::endl;
  glBufferData(GL_ARRAY_BUFFER, sizeof(BOX_POINTS), BOX_POINTS, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(0, box_VBO_IDs[POSITIONS_VBO_IDX], 0, 6 * sizeof(GL_FLOAT));
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
  }
  else {
    glVertexAttribPointer(
      0, 3,
      GL_FLOAT, GL_FALSE,
      6 * sizeof(GL_FLOAT), nullptr);
  }

  // Normals
  glEnableVertexAttribArray(2);
  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(2, box_VBO_IDs[POSITIONS_VBO_IDX], 3 * sizeof(GL_FLOAT), 6 * sizeof(GL_FLOAT));
    glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(2, 2);
  }
  else {
    glVertexAttribPointer(
      2, 3,
      GL_FLOAT, GL_FALSE,
      6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
  }

  // Colors

  //Color VBO Initialization
  glBindBuffer(GL_ARRAY_BUFFER, box_VBO_IDs[COLORS_VBO_IDX]);
  std::cout << "Scene::Init -> Allocated buffer memory for ID = ";
  std::cout << box_VBO_IDs[COLORS_VBO_IDX] << std::endl;
  glBufferData(GL_ARRAY_BUFFER, sizeof(BOX_COLOR), BOX_COLOR, GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(1, box_VBO_IDs[COLORS_VBO_IDX], 0, 4 * sizeof(unsigned char));
    glVertexAttribFormat(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0);
    glVertexAttribBinding(1, 1);
  }
  else {
    glVertexAttribPointer(
      1, 4,
      GL_UNSIGNED_BYTE, GL_TRUE,
      4 * sizeof(unsigned char), nullptr);
  }
}

void UpdateMaterialUniform() {
  shader::module::SetUniformVec3(shader_program_ID, "material.ambientFactor", box_material.ambientFactor);
  shader::module::SetUniformVec3(shader_program_ID, "material.diffuseFactor", box_material.diffuseFactor);
  shader::module::SetUniformVec3(shader_program_ID, "material.specularFactor", box_material.specularFactor);
  shader::module::SetUniformFloat(shader_program_ID, "material.shininessFactor", box_material.shininessFactor);
}
}

void Init(bool a_isDebug) {
  std::call_once(init_flag, [&]() {
    debug_mode = a_isDebug;

    skybox::LoadSkyBox();

    shader::factory::CompileShaderFile("scene.vert", GL_VERTEX_SHADER);
    shader::factory::CompileShaderFile("default.frag", GL_FRAGMENT_SHADER);
    shader_program_ID = shader::factory::CreateProgram();

    // Register light uniforms
    shader::module::Use(shader_program_ID); // Got to use the program before setting uniforms
    shader::module::SetUniformVec3(shader_program_ID, "ambient_light_color", { 1.0f, 1.0f, 1.0f });
    shader::module::SetUniformFloat(shader_program_ID, "ambient_light_intensity", 0.5f);

    // Color of the box is sent through the material
    box_material.ambientFactor    = { 1.0f, 0.5f, 0.31f }; 
    box_material.diffuseFactor    = { 1.0f, 0.5f, 0.31f };
    box_material.specularFactor   = { 0.5f, 0.5f, 0.5f };
    box_material.shininessFactor  = 32.0f;
    UpdateMaterialUniform();
    shader::module::Detach();
    
    // Should already be initialized but just in case
    light::module::Init();

    // Add lights in the scene in the SSBO
    light::Light wFrontLight, wBackLight;
    wFrontLight.position = { 0.0f, 0.0f, 2.0f, 0.0f };
    wFrontLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    wFrontLight.intensity = 1.0f;
    wFrontLight.attenuation = 0.0f;
    wFrontLight.radius = 10.0f;

    wBackLight.position = { 2.0f, 0.0f, 0.0f, 0.0f };
    wBackLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    wBackLight.intensity = 1.0f;
    wBackLight.attenuation = 0.0f;
    wBackLight.radius = 10.0f;

    light::module::AddLight(wFrontLight);
    light::module::AddLight(wBackLight);

    glGenVertexArrays(2, vertex_array_IDs);
    std::cout << "Scene::Init -> Generated VAO IDs = ";
    std::cout << vertex_array_IDs[0] << " & " << vertex_array_IDs[1] << " respectively for axes and box." << std::endl;

    glBindVertexArray(vertex_array_IDs[0]);
    std::cout << "Scene::Init -> Allocated array memory for ID = ";
    std::cout << vertex_array_IDs[0] << std::endl;
    InitializeAxes();

    glBindVertexArray(vertex_array_IDs[1]);
    std::cout << "Scene::Init -> Allocated array memory for ID = ";
    std::cout << vertex_array_IDs[1] << std::endl;
    InitializeTestingBox();

    glBindVertexArray(0);
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
    skybox::Destroy();
    if (axes_VBO_IDs[POSITIONS_VBO_IDX] != 0) {
      std::cout << "scene::Terminate -> Deallocating vertex VBO" << std::endl;
      glDeleteBuffers(1, &axes_VBO_IDs[POSITIONS_VBO_IDX]);
      axes_VBO_IDs[POSITIONS_VBO_IDX] = 0;
    }
    if (axes_VBO_IDs[COLORS_VBO_IDX] != 0) {
      std::cout << "scene::Terminate -> Deallocating color VBO" << std::endl;
      glDeleteBuffers(1, &axes_VBO_IDs[COLORS_VBO_IDX]);
      axes_VBO_IDs[COLORS_VBO_IDX] = 0;
    }
    if (box_VBO_IDs[POSITIONS_VBO_IDX] != 0) {
      std::cout << "scene::Terminate -> Deallocating vertex VBO" << std::endl;
      glDeleteBuffers(1, &box_VBO_IDs[POSITIONS_VBO_IDX]);
      box_VBO_IDs[POSITIONS_VBO_IDX] = 0;
    }
    if (box_VBO_IDs[COLORS_VBO_IDX] != 0) {
      std::cout << "scene::Terminate -> Deallocating color VBO" << std::endl;
      glDeleteBuffers(1, &box_VBO_IDs[COLORS_VBO_IDX]);
      box_VBO_IDs[COLORS_VBO_IDX] = 0;
    }

    // Should already be done, but just in case
    light::module::Terminate();
  });
}

void UpdateMaterial(const light::Material& a_material) { box_material = a_material; }
light::Material GetMaterial() { return box_material; }

bool IsDebug() { return debug_mode; }
void SetDebugOption(bool a_isDebug) { debug_mode = a_isDebug; }

void Render() {
  skybox::Render();
  if (debug_mode) {
    shader::module::Use(shader_program_ID);
    UpdateMaterialUniform();
    DrawDebugObjects();
    shader::module::Detach();
  }
}
} /* namespace scene */
} /* namespace glass_project */
} /* namespace particle */
} /* namespace gem */
