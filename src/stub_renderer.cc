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
#include "stub_renderer.hh"

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Gem {
namespace Particle {

StubRenderer::StubRenderer() {
  // VAO initialization
  glGenVertexArrays(1, &m_vertexArrayID);
  glBindVertexArray(m_vertexArrayID);

  // VBO initialization
  glGenBuffers(1, &m_vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
  glBufferData(GL_ARRAY_BUFFER,
    sizeof(glm::f32vec3),
    &(a_pParticlePool->m_position[0]),
    GL_STATIC_DRAW);
}

StubRenderer::~StubRenderer() {
  glDeleteBuffers(1, &m_vertexBufferID);
}

void StubRenderer::Render(std::unique_ptr<Pool> a_pParticlePool) {
  
}

} /* namespace Particle */
} /* namespace Gem */
