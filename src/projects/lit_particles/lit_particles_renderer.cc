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
#include "projects/lit_particles/lit_particles_renderer.hh"

// shader utilities
#include "utils/shader_factory.hh"
#include "utils/shader_module.hh"

// texture utilities
#include "utils/texture_factory.hh"
#include "utils/texture_module.hh"

#include "utils/imgui/imgui_log.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace gem { namespace particle {
namespace lit_particles_project {
TextureCoreGLRenderer::TextureCoreGLRenderer(const std::string& a_sTexturePath, float a_fParticleSize) {
  shader::factory::CompileShaderFile("particle_billboard.vert", GL_VERTEX_SHADER);
  shader::factory::CompileShaderFile("particle_billboard.geom", GL_GEOMETRY_SHADER);
  shader::factory::CompileShaderFile("particles.frag", GL_FRAGMENT_SHADER);
  m_shaderProgram = shader::factory::CreateProgram();

  shader::module::Use(m_shaderProgram);
  shader::module::SetUniformFloat(m_shaderProgram,"particle_size", a_fParticleSize);
  shader::module::Detach();

  // VAO initialization
  glGenVertexArrays(1, &m_vertexArrayID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::TextureCoreGLRenderer -> Generated VAO ID = %d\n", m_vertexArrayID);
  glBindVertexArray(m_vertexArrayID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::TextureCoreGLRenderer -> Allocated array memory for ID = %d\n", m_vertexArrayID);

  ParticleTexturesInit(a_sTexturePath);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

TextureCoreGLRenderer::~TextureCoreGLRenderer() {
  if (m_colorVBOID != 0) {
    ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::~TextureCoreGLRenderer -> Deallocating color VBO\n");
    glDeleteBuffers(1, &m_colorVBOID);
    m_colorVBOID = 0;
  }
  if (m_vertexBufferID != 0) {
    ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::~TextureCoreGLRenderer -> Deallocating vertex VBO\n");
    glDeleteBuffers(1, &m_vertexBufferID);
    m_vertexBufferID = 0;
  }
}

void TextureCoreGLRenderer::Bind(const std::shared_ptr<ParticlePool<CoreParticles> > &a_pPool) {
  Renderer<CoreParticles>::Bind(a_pPool);
  glBindVertexArray(m_vertexArrayID);
  ParticlePositionsInit(m_pPool);
  ParticleColorsInit(m_pPool);
  ParticleDirectionInit(m_pPool);
  glBindVertexArray(0);
}

void TextureCoreGLRenderer::ParticlePositionsInit(
  const std::shared_ptr<ParticlePool<CoreParticles> > & a_pPool) {
  // Positions VBO initialization
  glGenBuffers(1, &m_vertexBufferID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticlePositionsInit -> Generated vertex VBO ID = %d\n", m_vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticlePositionsInit -> Allocated buffer memory for ID = %d\n", m_vertexBufferID);

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
      sizeof(glm::f32vec3), nullptr);
  }
}

void TextureCoreGLRenderer::ParticleColorsInit(
  const std::shared_ptr<ParticlePool<CoreParticles> > & a_pPool) {
  //Color VBO Initialization
  glGenBuffers(1, &m_colorVBOID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticleColorsInit -> Generated color VBO ID = %d\n", m_colorVBOID);
  glBindBuffer(GL_ARRAY_BUFFER, m_colorVBOID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticleColorsInit -> Allocated buffer memory for ID = %d\n", m_colorVBOID);

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
      sizeof(glm::u8vec4), nullptr);
  }
}
void TextureCoreGLRenderer::ParticleDirectionInit(
  const std::shared_ptr<ParticlePool<CoreParticles> > & a_pPool) {
  //Direction VBO Initialization
  glGenBuffers(1, &m_directionVBOID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticleColorsInit -> Generated direction VBO ID = %d\n", m_directionVBOID);
  glBindBuffer(GL_ARRAY_BUFFER, m_directionVBOID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticleColorsInit -> Allocated buffer memory for ID = %d\n", m_directionVBOID);

  const std::size_t wParticleCount = a_pPool->GetParticleCount();

  glBufferData(GL_ARRAY_BUFFER,
    sizeof(glm::f32vec3)*wParticleCount,
    a_pPool->pCoreData->m_velocity.get(),
    GL_STATIC_DRAW);

  glEnableVertexAttribArray(2);

  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(2, m_directionVBOID, 0, sizeof(glm::f32vec3));
    glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(2, 2);
  }
  else {
    glVertexAttribPointer(
      2, 3,
      GL_FLOAT, GL_TRUE,
      sizeof(glm::f32vec3), nullptr);
  }
}

void TextureCoreGLRenderer::ParticleTexturesInit(const std::string& a_sTexturePath) {
  m_textureID = texture::factory::Create2DTexture(a_sTexturePath);
  shader::module::GetUniformLocation(m_shaderProgram, "mytexture");
}

void TextureCoreGLRenderer::Update() {
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

    glBindBuffer(GL_ARRAY_BUFFER, m_directionVBOID);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
      sizeof(glm::f32vec3)*wActiveParticleCount,
      m_pPool->pCoreData->m_velocity.get());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}
void TextureCoreGLRenderer::Render() {
  shader::module::Use(m_shaderProgram);
  glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_COLOR, GL_ONE); // Useful for White on Black textures
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
  shader::module::SetUniformBool(m_shaderProgram,"mytexture", 0);

  glBindVertexArray(m_vertexArrayID);
  const std::size_t count = m_pPool->GetActiveParticleCount();
  if (count > 0) {
    glDrawArrays(GL_POINTS, 0, (GLsizei)count);
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);
}
} /* namespace lit_particles_project */
} /* namespace particle */
} /* namespace gem */
