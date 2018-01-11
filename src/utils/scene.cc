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
#include "utils/scene.hh"

#include <mutex>

#include <GL/glew.h>

// shader utilities
#include "utils/shader_factory.hh"
#include "utils/shader_module.hh"

// logger utility
#include "utils/imgui/imgui_log.h"

namespace gem {
namespace particle {
namespace scene {
namespace {
const GLfloat AXES_POINTS[] = {
  0.0f,0.0f,0.0f,
  1.0f,0.0f,0.0f,
  0.0f,0.0f,0.0f,
  0.0f,1.0f,0.0f,
  0.0f,0.0f,0.0f,
  0.0f,0.0f,1.0f
};

bool    debug_mode;
GLuint  vertex_array_ID;
GLuint  vertex_buffer_ID;
GLuint  shader_program_ID;

std::once_flag init_flag;
std::once_flag terminate_flag;

void DrawAxes() {
  shader::module::Use(shader_program_ID);
  glBindVertexArray(vertex_array_ID);
  glDrawArrays(GL_LINES, 0, 2);
  glDrawArrays(GL_LINES, 2, 2);
  glDrawArrays(GL_LINES, 4, 2);
  glBindVertexArray(0);
}
}

void Init(bool a_isDebug) {
  std::call_once(init_flag, [&]() {
    debug_mode = a_isDebug;

    shader::factory::CompileShaderFile("debug_axes.vert", GL_VERTEX_SHADER);
    shader::factory::CompileShaderFile("default.frag", GL_FRAGMENT_SHADER);
    shader_program_ID = shader::factory::CreateProgram();

    glGenVertexArrays(1, &vertex_array_ID);
    ImGuiLog::GetInstance().AddLog("Scene::Init -> Generated VAO ID = %d\n", vertex_array_ID);
    glBindVertexArray(vertex_array_ID);
    ImGuiLog::GetInstance().AddLog("Scene::Init -> Allocated array memory for ID = %d\n", vertex_array_ID);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vertex_buffer_ID);
    ImGuiLog::GetInstance().AddLog("Scene::Init -> Generated color VBO ID = %d\n", vertex_buffer_ID);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ID);
    ImGuiLog::GetInstance().AddLog("Scene::Init -> Allocated buffer memory for ID = %d\n", vertex_buffer_ID);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), AXES_POINTS, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
    if (vertex_buffer_ID != 0) {
      ImGuiLog::GetInstance().AddLog("scene::Terminate->Deallocating vertex VBO\n");
      glDeleteBuffers(1, &vertex_buffer_ID);
      vertex_buffer_ID = 0;
    }
  });
}

bool IsDebug() { return debug_mode; }
void SetDebugOption(bool a_isDebug) { debug_mode = a_isDebug; }

void Render() {
  if (debug_mode) {
    DrawAxes();
  }
}
} /* namespace scene */
} /* namespace particle */
} /* namespace gem */
