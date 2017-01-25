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

#include <iostream>
namespace Gem {
namespace Particle {
StubRenderer::StubRenderer(){
  Renderer::m_pParticlePool = nullptr;
}
void StubRenderer::InitImpl() {
  //Color VBO Initialization
  glGenBuffers(1, &m_colorVBOID);
  glBindBuffer(GL_ARRAY_BUFFER, m_colorVBOID);

  const std::size_t wParticleCount = m_pParticlePool->GetParticleCount();

  glBufferData(GL_ARRAY_BUFFER,
    sizeof(glm::u8vec4)*wParticleCount,
    m_pParticlePool->m_color.get(),
    GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);

  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(1, m_colorVBOID, 0, sizeof(glm::u8vec4));
    glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(1, 1);
  }
  else {
    glVertexAttribPointer(
      1, 4,
      GL_FLOAT, GL_FALSE,
      sizeof(glm::u8vec4), (void *)0);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void StubRenderer::TerminateImpl() {
  if (m_colorVBOID != 0) {
    glDeleteBuffers(1, &m_colorVBOID);
    m_colorVBOID = 0;
  }
}
void StubRenderer::Update() {
  const std::size_t wActiveParticleCount = 
    m_pParticlePool->GetActiveParticleCount();

  // TODO: See if the "if" branching is even necessary here
  // (test performance)
  if (wActiveParticleCount > 0)
  {
//    std::cout << "sizeof(glm::f32vec3)*wActiveParticleCount = " << sizeof(glm::f32vec3)*wActiveParticleCount << std::endl;
    std::cout << "&(m_pParticlePool->m_position) = " << &(m_pParticlePool->m_position) << std::endl;
    std::cout << "&(m_pParticlePool->m_position[0]) = " << &(m_pParticlePool->m_position[0]) << std::endl;
    std::cout << "m_pParticlePool->m_position.get() = " << m_pParticlePool->m_position.get() << std::endl;

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 
      sizeof(glm::f32vec3)*wActiveParticleCount, 
      m_pParticlePool->m_position.get());

    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBOID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 
      sizeof(glm::u8vec4)*wActiveParticleCount,
      m_pParticlePool->m_color.get());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}
void StubRenderer::Render() {
  glBindVertexArray(m_vertexArrayID);

  const std::size_t count = m_pParticlePool->GetActiveParticleCount();
  if (count > 0)
    glDrawArrays(GL_POINTS, 0, count); // TODO: Put something to change the points for quads as desired

  glBindVertexArray(0);
}
} /* namespace Particle */
} /* namespace Gem */
