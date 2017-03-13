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
#include "scene.hh"

#include <iostream>

#include "shader.hh"

namespace gem {
namespace particle {
const GLfloat Scene::AXES_POINTS[] = {
  0.0f,0.0f,0.0f,
  1.0f,0.0f,0.0f,
  0.0f,0.0f,0.0f,
  0.0f,1.0f,0.0f,
  0.0f,0.0f,0.0f,
  0.0f,0.0f,1.0f
};

Scene::Scene(bool a_isDebug)
  : m_bIsDebug(a_isDebug) {
  shader_manager::CompileShaderFile("shaders/debug_axes.vert", GL_VERTEX_SHADER);
  shader_manager::CompileShaderFile("shaders/default.frag", GL_FRAGMENT_SHADER);
  m_unProgramID = shader_manager::CreateProgram();

  glGenVertexArrays(1, &m_vertexArrayID);
  glBindVertexArray(m_vertexArrayID);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &m_vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), AXES_POINTS, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

Scene::~Scene() {
  if (m_vertexBufferID != 0) {
    std::cout << "Scene::~Scene -> Deallocating vertex VBO" << std::endl;
    glDeleteBuffers(1, &m_vertexBufferID);
    m_vertexBufferID = 0;
  }
}

void Scene::Render() {
  if (m_bIsDebug) {
    DrawAxes();
  }
}

void Scene::DrawAxes() {
  shader_manager::Use(m_unProgramID);
  glBindVertexArray(m_vertexArrayID);
  glDrawArrays(GL_LINES, 0, 2);
  glDrawArrays(GL_LINES, 2, 2);
  glDrawArrays(GL_LINES, 4, 2);
  glBindVertexArray(0);
}
} /* namespace particle */
} /* namespace gem */
