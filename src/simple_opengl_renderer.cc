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
#include "simple_opengl_renderer.hh"

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <iostream>
namespace gem {
namespace particle {
SimpleGLRenderer::SimpleGLRenderer(const std::shared_ptr<ParticlePool<CoreParticles> > & a_pPool)
  : GLRenderer(a_pPool){
  //Color VBO Initialization
  glGenBuffers(1, &m_colorVBOID);
  std::cout << "SimpleOpenGLRenderer::SimpleGLRenderer -> Generated color VBO ID = ";
  std::cout << m_colorVBOID << std::endl;
  glBindBuffer(GL_ARRAY_BUFFER, m_colorVBOID);
  std::cout << "SimpleOpenGLRenderer::SimpleGLRenderer -> Allocated buffer memory for ID = ";
  std::cout << m_colorVBOID << std::endl;

  const std::size_t wParticleCount = a_pPool->GetParticleCount();

  glBufferData(GL_ARRAY_BUFFER,
    sizeof(glm::u8vec4)*wParticleCount,
    a_pPool->pCoreData->m_color.get(),
    GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);

  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(1, m_colorVBOID, 0, sizeof(glm::u8vec4));
    glVertexAttribFormat(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0);
    glVertexAttribBinding(1, 1);
  }
  else {
    glVertexAttribPointer(
      1, 4,
      GL_UNSIGNED_BYTE, GL_FALSE,
      sizeof(glm::u8vec4), (void *)0);
  }
  // TODO: See if following is really necessary
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

SimpleGLRenderer::~SimpleGLRenderer() {
  if (m_colorVBOID != 0) {
    std::cout << "SimpleOpenGLRenderer::~SimpleOpenGLRenderer -> Deallocating color VBO" << std::endl;
    glDeleteBuffers(1, &m_colorVBOID);
    m_colorVBOID = 0;
  }
}

void SimpleGLRenderer::Update(const std::shared_ptr<ParticlePool<CoreParticles> > &a_pPool) {
  const std::size_t wActiveParticleCount =
    a_pPool->GetActiveParticleCount();

  // TODO: See if the "if" branching is even necessary here
  // (test performance)
  if (wActiveParticleCount > 0) {
    //std::cout << "Active particles : " << wActiveParticleCount << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 
      sizeof(glm::f32vec3)*wActiveParticleCount, 
      a_pPool->pCoreData->m_position.get());

    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBOID);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
      sizeof(glm::u8vec4)*wActiveParticleCount,
      a_pPool->pCoreData->m_color.get());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}
void SimpleGLRenderer::Render(const std::shared_ptr<ParticlePool<CoreParticles> > &a_pPool) {
  glBindVertexArray(m_vertexArrayID);
  const std::size_t count = a_pPool->GetActiveParticleCount();
  if (count > 0) {
    glDrawArrays(GL_POINTS, 0, (GLsizei)count); // TODO: Put something to change the points for quads as desired
  }
  glBindVertexArray(0);
}
} /* namespace particle */
} /* namespace gem */
