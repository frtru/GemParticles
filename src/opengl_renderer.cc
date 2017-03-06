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

namespace gem {
namespace particle {
GLRenderer::GLRenderer(const std::shared_ptr<ParticlePool> & a_pPool){
	// VAO initialization
	glGenVertexArrays(1, &m_vertexArrayID);
	std::cout << "GLRenderer::GLRenderer -> Generated VAO ID = ";
	std::cout << m_vertexArrayID << std::endl;
	glBindVertexArray(m_vertexArrayID);
	std::cout << "GLRenderer::GLRenderer -> Allocated array memory for ID = ";
	std::cout << m_vertexArrayID << std::endl;

	// VBO initialization
	glGenBuffers(1, &m_vertexBufferID);
	std::cout << "GLRenderer::GLRenderer -> Generated vertex VBO ID = ";
	std::cout << m_vertexBufferID << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	std::cout << "GLRenderer::GLRenderer -> Allocated buffer memory for ID = ";
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

GLRenderer::~GLRenderer() {
  if (m_vertexBufferID != 0) {
	  std::cout << "GLRenderer::~GLRenderer -> Deallocating vertex VBO" << std::endl;
	  glDeleteBuffers(1, &m_vertexBufferID);
	  m_vertexBufferID = 0;
  }
}
} /* namespace particle */
} /* namespace gem */
