/*************************************************************************
 * Copyright (c) 2016 Francois Trudel
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
#include "renderers/core_opengl_renderer.hh"

// shader utilities
#include "utils/shader_factory.hh"
#include "utils/shader_module.hh"

// texture utilities
#include "utils/texture_factory.hh"
#include "utils/texture_module.hh"

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <iostream>
namespace gem {
namespace particle {
CoreGLRenderer::CoreGLRenderer() {
  shader::factory::CompileShaderFile("shaders/default.vert", GL_VERTEX_SHADER);
  shader::factory::CompileShaderFile("shaders/default.frag", GL_FRAGMENT_SHADER);
  m_shaderProgram = shader::factory::CreateProgram();

  // VAO initialization
  glGenVertexArrays(1, &m_vertexArrayID);
  std::cout << "CoreGLRenderer::CoreGLRenderer -> Generated VAO ID = ";
  std::cout << m_vertexArrayID << std::endl;
  glBindVertexArray(m_vertexArrayID);
  std::cout << "CoreGLRenderer::CoreGLRenderer -> Allocated array memory for ID = ";
  std::cout << m_vertexArrayID << std::endl;

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

CoreGLRenderer::~CoreGLRenderer() {
  if (m_colorVBOID != 0) {
    std::cout << "CoreGLRenderer::~CoreGLRenderer -> Deallocating color VBO" << std::endl;
    glDeleteBuffers(1, &m_colorVBOID);
    m_colorVBOID = 0;
  }
  if (m_vertexBufferID != 0) {
    std::cout << "CoreGLRenderer::~CoreGLRenderer -> Deallocating vertex VBO" << std::endl;
    glDeleteBuffers(1, &m_vertexBufferID);
    m_vertexBufferID = 0;
  }
}

void CoreGLRenderer::Bind(const std::shared_ptr<ParticlePool<CoreParticles> > &a_pPool) {
  Renderer<CoreParticles>::Bind(a_pPool);
  glBindVertexArray(m_vertexArrayID);
  ParticlePositionsInit(m_pPool);
  ParticleColorsInit(m_pPool);
  glBindVertexArray(0);
}

void CoreGLRenderer::ParticlePositionsInit(
  const std::shared_ptr<ParticlePool<CoreParticles> > & a_pPool) {
  // Positions VBO initialization
  glGenBuffers(1, &m_vertexBufferID);
  std::cout << "CoreGLRenderer::CoreGLRenderer -> Generated vertex VBO ID = ";
  std::cout << m_vertexBufferID << std::endl;
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
  std::cout << "CoreGLRenderer::CoreGLRenderer -> Allocated buffer memory for ID = ";
  std::cout << m_vertexBufferID << std::endl;

  const std::size_t wParticleCount = a_pPool->GetParticleCount();

  glBufferData(GL_ARRAY_BUFFER,
    sizeof(glm::f32vec3)*wParticleCount,
    a_pPool->pCoreData->m_position.get(),
    GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);

  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(0, m_vertexBufferID, 0, sizeof(glm::f32vec3));
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
  }
  else {
    glVertexAttribPointer(
      0, 3,
      GL_FLOAT, GL_FALSE,
      sizeof(glm::f32vec3), (void *)0);
  }
}

void CoreGLRenderer::ParticleColorsInit(
  const std::shared_ptr<ParticlePool<CoreParticles> > & a_pPool) {
  //Color VBO Initialization
  glGenBuffers(1, &m_colorVBOID);
  std::cout << "CoreGLRenderer::CoreGLRenderer -> Generated color VBO ID = ";
  std::cout << m_colorVBOID << std::endl;
  glBindBuffer(GL_ARRAY_BUFFER, m_colorVBOID);
  std::cout << "CoreGLRenderer::CoreGLRenderer -> Allocated buffer memory for ID = ";
  std::cout << m_colorVBOID << std::endl;

  const std::size_t wParticleCount = a_pPool->GetParticleCount();

  glBufferData(GL_ARRAY_BUFFER,
    sizeof(glm::u8vec4)*wParticleCount,
    a_pPool->pCoreData->m_color.get(),
    GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);

  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(1, m_colorVBOID, 0, sizeof(glm::u8vec4));
    glVertexAttribFormat(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0);
    glVertexAttribBinding(1, 1);
  }
  else {
    glVertexAttribPointer(
      1, 4,
      GL_UNSIGNED_BYTE, GL_TRUE,
      sizeof(glm::u8vec4), (void *)0);
  }
}

void CoreGLRenderer::Update() {
  shader::module::Use(m_shaderProgram);
  const std::size_t wActiveParticleCount =
    m_pPool->GetActiveParticleCount();

  // TODO: See if the "if" branching is even necessary here
  // (test performance)
  if (wActiveParticleCount > 0) {
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 
      sizeof(glm::f32vec3)*wActiveParticleCount, 
      m_pPool->pCoreData->m_position.get());

    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBOID);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
      sizeof(glm::u8vec4)*wActiveParticleCount,
      m_pPool->pCoreData->m_color.get());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}
void CoreGLRenderer::Render() {
  shader::module::Use(m_shaderProgram);

  glBindVertexArray(m_vertexArrayID);
  const std::size_t count = m_pPool->GetActiveParticleCount();
  if (count > 0) {
    glDrawArrays(GL_POINTS, 0, (GLsizei)count);
  }
  glBindVertexArray(0);
}
} /* namespace particle */
} /* namespace gem */
