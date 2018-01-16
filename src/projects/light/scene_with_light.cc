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
#include "projects/light/scene_with_light.hh"

#include <mutex>

#include <GL/glew.h>

#include "utils/shader_factory.hh"
#include "utils/shader_module.hh"
#include "utils/light_module.hh"

#include "utils/imgui/imgui_property_editor.h"
#include "utils/imgui/imgui_log.h"


namespace gem { namespace particle {
namespace light_project {
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

const GLfloat WALL_AND_FLOOR_POINTS[] = {
  -1500.0f, 500.0f, -10.0f,  0.0f,  0.0f, 1.0f,
  -1500.0f, -1.0f, -10.0f,  0.0f,  0.0f, 1.0f,
  1500.0f, -1.0f, -10.0f,  0.0f,  0.0f, 1.0f,
  1500.0f, -1.0f, -10.0f,  0.0f,  0.0f, 1.0f,
  1500.0f, 500.0f, -10.0f,  0.0f,  0.0f, 1.0f,
  -1500.0f, 500.0f, -10.0f,  0.0f,  0.0f, 1.0f,

  -1500.0f, -1.0f,  -10.0f,  0.0f,  1.0f,  0.0f,
  -1500.0f, -1.0f,  100.0f,  0.0f,  1.0f,  0.0f,
  1500.0f, -1.0f,  100.0f,  0.0f,  1.0f,  0.0f,
  1500.0f, -1.0f,  100.0f,  0.0f,  1.0f,  0.0f,
  1500.0f, -1.0f,  -10.0f,  0.0f,  1.0f,  0.0f,
  -1500.0f, -1.0f,  -10.0f,  0.0f,  1.0f,  0.0f,
};

// NOTE : With the material being sent through with an uniform,
// This is actually not being used. The color is sent in the material directly.
// According to : https://www.opengl.org/discussion_boards/showthread.php/171379-VBOs-Drawing-vertices-of-same-color
// "You have to specify the color for each vertex."
const unsigned char BOX_COLOR[] = {
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
    200u,200u,200u,255u,
};

const unsigned char WALL_AND_FLOOR_COLOR[] = {
  200u,200u,200u,255u,
  200u,200u,200u,255u,
  200u,200u,200u,255u,
  200u,200u,200u,255u,
  200u,200u,200u,255u,
  200u,200u,200u,255u,
  200u,200u,200u,255u,
  200u,200u,200u,255u,
  200u,200u,200u,255u,
  200u,200u,200u,255u,
  200u,200u,200u,255u,
  200u,200u,200u,255u
};

bool    debug_mode;
GLuint  vertex_array_IDs[2];

const std::size_t POSITIONS_VBO_IDX = 0;
const std::size_t COLORS_VBO_IDX = 1;
//const std::size_t NORMALS_VBO_IDX = 2;

GLuint  axes_VBO_IDs[2];
GLuint  box_VBO_IDs[2];
GLuint  wall_and_floor_VBO_IDs[2];
GLuint  shader_program_ID;

std::once_flag init_flag;
std::once_flag terminate_flag;

light::Material material;

void DrawDebugObjects() {
  glBindVertexArray(vertex_array_IDs[0]);
  glDrawArrays(GL_LINES, 0, 2);
  glDrawArrays(GL_LINES, 2, 2);
  glDrawArrays(GL_LINES, 4, 2);
  glBindVertexArray(vertex_array_IDs[1]);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

void DrawWallAndFloor() {
  glBindVertexArray(vertex_array_IDs[2]);
  glDrawArrays(GL_TRIANGLES, 0, 12);
  glBindVertexArray(0);
}

void InitializeAxes() {
  glGenBuffers(2, axes_VBO_IDs);
  ImGuiLog::GetInstance().AddLog("Scene::InitializeAxes->Generated axes VBOs, IDs = %d & %d\n", 
    axes_VBO_IDs[POSITIONS_VBO_IDX], axes_VBO_IDs[COLORS_VBO_IDX]);

  // Positions
  glBindBuffer(GL_ARRAY_BUFFER, axes_VBO_IDs[POSITIONS_VBO_IDX]);
  ImGuiLog::GetInstance().AddLog("Scene::InitializeAxes -> Allocated buffer memory for ID = %d\n", 
    axes_VBO_IDs[POSITIONS_VBO_IDX]);
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
  ImGuiLog::GetInstance().AddLog("Scene::InitializeAxes -> Allocated buffer memory for ID = %d\n",
    axes_VBO_IDs[COLORS_VBO_IDX]);
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
  ImGuiLog::GetInstance().AddLog("Scene::InitializeTestingBox -> Generated 2 VBOs, IDs = %d & %d\n",
    box_VBO_IDs[POSITIONS_VBO_IDX], box_VBO_IDs[COLORS_VBO_IDX]);

  // Positions
  glBindBuffer(GL_ARRAY_BUFFER, box_VBO_IDs[POSITIONS_VBO_IDX]);
  ImGuiLog::GetInstance().AddLog("Scene::InitializeTestingBox -> Allocated buffer memory for ID = %d\n",
    box_VBO_IDs[POSITIONS_VBO_IDX]);
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
  ImGuiLog::GetInstance().AddLog("Scene::InitializeTestingBox -> Allocated buffer memory for ID = %d\n",
    box_VBO_IDs[COLORS_VBO_IDX]);
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
void InitializeWallAndFloor() {
  glGenBuffers(2, wall_and_floor_VBO_IDs);
  ImGuiLog::GetInstance().AddLog("Scene::InitializeWallAndFloor -> Generated 2 VBOs, IDs = %d & %d\n",
    wall_and_floor_VBO_IDs[POSITIONS_VBO_IDX], wall_and_floor_VBO_IDs[COLORS_VBO_IDX]);

  // Positions
  glBindBuffer(GL_ARRAY_BUFFER, wall_and_floor_VBO_IDs[POSITIONS_VBO_IDX]);
  ImGuiLog::GetInstance().AddLog("Scene::InitializeWallAndFloor -> Allocated buffer memory for ID = %d\n",
    wall_and_floor_VBO_IDs[POSITIONS_VBO_IDX]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(WALL_AND_FLOOR_POINTS), WALL_AND_FLOOR_POINTS, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(0, wall_and_floor_VBO_IDs[POSITIONS_VBO_IDX], 0, 6 * sizeof(GL_FLOAT));
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
    glBindVertexBuffer(2, wall_and_floor_VBO_IDs[POSITIONS_VBO_IDX], 3 * sizeof(GL_FLOAT), 6 * sizeof(GL_FLOAT));
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
  glBindBuffer(GL_ARRAY_BUFFER, wall_and_floor_VBO_IDs[COLORS_VBO_IDX]);
  ImGuiLog::GetInstance().AddLog("Scene::InitializeWallAndFloor -> Allocated buffer memory for ID = %d\n",
    wall_and_floor_VBO_IDs[COLORS_VBO_IDX]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(WALL_AND_FLOOR_COLOR), WALL_AND_FLOOR_COLOR, GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(1, wall_and_floor_VBO_IDs[COLORS_VBO_IDX], 0, 4 * sizeof(unsigned char));
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
  shader::module::SetUniformVec3(shader_program_ID, "material.ambientFactor", material.ambientFactor);
  shader::module::SetUniformVec3(shader_program_ID, "material.diffuseFactor", material.diffuseFactor);
  shader::module::SetUniformVec3(shader_program_ID, "material.specularFactor", material.specularFactor);
  shader::module::SetUniformFloat(shader_program_ID, "material.shininessFactor", material.shininessFactor);
}
}

void Init(bool a_isDebug) {
  std::call_once(init_flag, [&]() {
    debug_mode = a_isDebug;
    glClearColor(0.78431372549019607843137254901961f, 0.78431372549019607843137254901961f, 0.78431372549019607843137254901961f, 1.0f);

    shader::factory::CompileShaderFile("scene.vert", GL_VERTEX_SHADER);
    shader::factory::CompileShaderFile("default.frag", GL_FRAGMENT_SHADER);
    shader_program_ID = shader::factory::CreateProgram();

    // Register light uniforms
    shader::module::Use(shader_program_ID); // Got to use the program before setting uniforms
    shader::module::SetUniformVec3(shader_program_ID, "ambient_light_color", { 1.0f, 1.0f, 1.0f });
    shader::module::SetUniformFloat(shader_program_ID, "ambient_light_intensity", 0.5f);

    // Color of the box is sent through the material
    material.ambientFactor    = { 0.25f, 0.25f, 0.25f };
    material.diffuseFactor    = { 0.15f, 0.15f, 0.15f };
    material.specularFactor   = { 0.3f, 0.3f, 0.3f };
    material.shininessFactor  = 4.0f;
    UpdateMaterialUniform();
    shader::module::Detach();
    
    // Should already be initialized but just in case
    light::module::Init();

    // Add lights in the scene in the SSBO
    light::Light wFrontLight, wBackLight;
    wFrontLight.position = { 0.0f, 0.0f, 2.0f, 0.0f };
    wFrontLight.color = { 0.5f, 0.5f, 0.5f, 0.5f };
    wFrontLight.intensity = 1.0f;
    wFrontLight.attenuation = 0.0f;
    wFrontLight.radius = 20.0f;

    wBackLight.position = { -40.0f, 0.0f, 0.0f, 0.0f };
    wBackLight.color = { 0.5f, 0.5f, 0.5f, 0.5f };
    wBackLight.intensity = 1.0f;
    wBackLight.attenuation = 0.0f;
    wBackLight.radius = 20.0f;

    light::module::AddLight(wFrontLight);
    light::module::AddLight(wBackLight);

    glGenVertexArrays(3, vertex_array_IDs);
    ImGuiLog::GetInstance().AddLog("Scene::Init -> Generated VAO IDs = %d & %d respectively for axes and box.\n",
      vertex_array_IDs[0], vertex_array_IDs[1]);

    glBindVertexArray(vertex_array_IDs[0]);
    ImGuiLog::GetInstance().AddLog("Scene::Init -> Allocated array memory for ID = %d\n", vertex_array_IDs[0]);
    InitializeAxes();

    glBindVertexArray(vertex_array_IDs[1]);
    ImGuiLog::GetInstance().AddLog("Scene::Init -> Allocated array memory for ID = %d\n", vertex_array_IDs[1]);
    InitializeTestingBox();

    glBindVertexArray(vertex_array_IDs[2]);
    ImGuiLog::GetInstance().AddLog("Scene::Init -> Allocated array memory for ID = %d\n", vertex_array_IDs[2]);
    InitializeWallAndFloor();
    glBindVertexArray(0);

    // Add properties to the editor
    ImGuiPropertyEditor &editor = ImGuiPropertyEditor::GetInstance();
    editor.AddObject("Box material", &material);
    editor.AddProperty<PropertyType::VEC3>("Ambient factor", &(material.ambientFactor), [&]() {
      shader::module::Use(shader_program_ID); // Got to use the program before setting uniforms
      UpdateMaterialUniform();
      shader::module::Detach();
    }, 0.05f);
    editor.AddProperty<PropertyType::VEC3>("Diffuse factor", &(material.diffuseFactor), [&]() {
      shader::module::Use(shader_program_ID); // Got to use the program before setting uniforms
      UpdateMaterialUniform();
      shader::module::Detach();
    }, 0.05f);
    editor.AddProperty<PropertyType::VEC3>("Specular factor", &(material.specularFactor), [&]() {
      shader::module::Use(shader_program_ID); // Got to use the program before setting uniforms
      UpdateMaterialUniform();
      shader::module::Detach();
    }, 0.05f);
    editor.AddProperty<PropertyType::DRAG_FLOAT>("Shininess factor", &(material.shininessFactor), [&]() {
      shader::module::Use(shader_program_ID); // Got to use the program before setting uniforms
      UpdateMaterialUniform();
      shader::module::Detach();
    }, 0.05f);
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
    if (axes_VBO_IDs[POSITIONS_VBO_IDX] != 0) {
      ImGuiLog::GetInstance().AddLog("scene::Terminate -> Deallocating vertex VBO\n");
      glDeleteBuffers(1, &axes_VBO_IDs[POSITIONS_VBO_IDX]);
      axes_VBO_IDs[POSITIONS_VBO_IDX] = 0;
    }
    if (axes_VBO_IDs[COLORS_VBO_IDX] != 0) {
      ImGuiLog::GetInstance().AddLog("scene::Terminate -> Deallocating color VBO\n");
      glDeleteBuffers(1, &axes_VBO_IDs[COLORS_VBO_IDX]);
      axes_VBO_IDs[COLORS_VBO_IDX] = 0;
    }
    if (box_VBO_IDs[POSITIONS_VBO_IDX] != 0) {
      ImGuiLog::GetInstance().AddLog("scene::Terminate -> Deallocating vertex VBO\n");
      glDeleteBuffers(1, &box_VBO_IDs[POSITIONS_VBO_IDX]);
      box_VBO_IDs[POSITIONS_VBO_IDX] = 0;
    }
    if (box_VBO_IDs[COLORS_VBO_IDX] != 0) {
      ImGuiLog::GetInstance().AddLog("scene::Terminate -> Deallocating color VBO\n");
      glDeleteBuffers(1, &box_VBO_IDs[COLORS_VBO_IDX]);
      box_VBO_IDs[COLORS_VBO_IDX] = 0;
    }
    if (wall_and_floor_VBO_IDs[POSITIONS_VBO_IDX] != 0) {
      ImGuiLog::GetInstance().AddLog("scene::Terminate -> Deallocating vertex VBO\n");
      glDeleteBuffers(1, &wall_and_floor_VBO_IDs[POSITIONS_VBO_IDX]);
      wall_and_floor_VBO_IDs[POSITIONS_VBO_IDX] = 0;
    }
    if (wall_and_floor_VBO_IDs[COLORS_VBO_IDX] != 0) {
      ImGuiLog::GetInstance().AddLog("scene::Terminate -> Deallocating color VBO\n");
      glDeleteBuffers(1, &wall_and_floor_VBO_IDs[COLORS_VBO_IDX]);
      wall_and_floor_VBO_IDs[COLORS_VBO_IDX] = 0;
    }
    // Should already be done, but just in case
    light::module::Terminate();
  });
}

void UpdateMaterial(const light::Material& a_material) { material = a_material; }
light::Material GetMaterial() { return material; }

bool IsDebug() { return debug_mode; }
void SetDebugOption(bool a_isDebug) { debug_mode = a_isDebug; }

void Render() {
  shader::module::Use(shader_program_ID);
  UpdateMaterialUniform();
  if (debug_mode)
    DrawDebugObjects(); 
  DrawWallAndFloor();
  shader::module::Detach();
}
} /* namespace scene */
} /* namespace light_project */
} /* namespace particle */
} /* namespace gem */
