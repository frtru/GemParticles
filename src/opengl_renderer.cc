/*************************************************************************
 * Copyright (c) 2016 François Trudel
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
#include "opengl_renderer.hh"

#include <iostream>

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Gem {
namespace Particle {
GLRenderer::GLRenderer() {
  // VAO initialization
  glGenVertexArrays(1, &m_vertexArrayID);
  glBindVertexArray(m_vertexArrayID);

  // VBO initialization
  glGenBuffers(1, &m_vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
}

GLRenderer::~GLRenderer() {
  if (m_vertexBufferID != 0) {
    Terminate();
  }
}

void GLRenderer::Init(Pool* a_pPool) {
  if (m_pParticlePool == nullptr) {

    m_pParticlePool = a_pPool;

    glBufferData(GL_ARRAY_BUFFER,
      sizeof(glm::f32vec3),
      &(m_pParticlePool->m_position[0]),
      GL_STATIC_DRAW);
  }
  else {
    std::cerr << "WARNING: GLRenderer::Init-> Renderer already initialized with another particle pool." << std::endl;
  }
}

void GLRenderer::Terminate() {
  glDeleteBuffers(1, &m_vertexBufferID);
  m_vertexBufferID = 0;
}
} /* namespace Particle */
} /* namespace Gem */
