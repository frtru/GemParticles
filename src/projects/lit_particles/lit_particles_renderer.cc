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
#include "utils/imgui/imgui_property_editor.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace gem { namespace particle {
namespace lit_particles_project {
LitParticlesRenderer::LitParticlesRenderer(const std::string& a_sTexturePath, float a_fParticleSize)
 : m_fParticleSize(a_fParticleSize) {
  shader::factory::CompileShaderFile("particle_billboard.vert", GL_VERTEX_SHADER);
  shader::factory::CompileShaderFile("particle_billboard.geom", GL_GEOMETRY_SHADER);
  shader::factory::CompileShaderFile("particles.frag", GL_FRAGMENT_SHADER);
  m_shaderProgram = shader::factory::CreateProgram();

  shader::module::Use(m_shaderProgram);
  shader::module::SetUniformFloat(m_shaderProgram,"particle_size", m_fParticleSize);
  shader::module::Detach();

  // VAO initialization
  glGenVertexArrays(1, &m_vertexArrayID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::TextureCoreGLRenderer -> Generated VAO ID = %d\n", m_vertexArrayID);
  glBindVertexArray(m_vertexArrayID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::TextureCoreGLRenderer -> Allocated array memory for ID = %d\n", m_vertexArrayID);

  ParticleTexturesInit(a_sTexturePath);

  ImGuiPropertyEditor &editor = ImGuiPropertyEditor::GetInstance();
  editor.AddObject("Particles renderer", this);
  editor.AddProperty<PropertyType::DRAG_FLOAT>("Particle size", &m_fParticleSize, [&]() {
    shader::module::Use(m_shaderProgram); // Got to use the program before setting uniforms
    shader::module::SetUniformFloat(m_shaderProgram, "particle_size", m_fParticleSize);
    shader::module::Detach();
  }, 0.05f);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

LitParticlesRenderer::~LitParticlesRenderer() {
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

void LitParticlesRenderer::Bind(const std::shared_ptr<ParticlePool<LitParticlesData> > &a_pPool) {
  Renderer<LitParticlesData>::Bind(a_pPool);
  glBindVertexArray(m_vertexArrayID);
  ParticlePositionsInit(m_pPool);
  ParticleColorsInit(m_pPool);
  ParticleDirectionInit(m_pPool);
  glBindVertexArray(0);
}

void LitParticlesRenderer::ParticlePositionsInit(
  const std::shared_ptr<ParticlePool<LitParticlesData> > & a_pPool) {
  // Positions VBO initialization
  glGenBuffers(1, &m_vertexBufferID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticlePositionsInit -> Generated vertex VBO ID = %d\n", m_vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticlePositionsInit -> Allocated buffer memory for ID = %d\n", m_vertexBufferID);

  const std::size_t wParticleCount = a_pPool->GetParticleCount();

  glBufferData(GL_ARRAY_BUFFER,
    sizeof(glm::f32vec3)*wParticleCount,
    a_pPool->pData->m_position.get(),
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

void LitParticlesRenderer::ParticleColorsInit(
  const std::shared_ptr<ParticlePool<LitParticlesData> > & a_pPool) {
  //Color VBO Initialization
  glGenBuffers(1, &m_colorVBOID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticleColorsInit -> Generated color VBO ID = %d\n", m_colorVBOID);
  glBindBuffer(GL_ARRAY_BUFFER, m_colorVBOID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticleColorsInit -> Allocated buffer memory for ID = %d\n", m_colorVBOID);

  const std::size_t wParticleCount = a_pPool->GetParticleCount();

  glBufferData(GL_ARRAY_BUFFER,
    sizeof(glm::u8vec4)*wParticleCount,
    a_pPool->pData->m_color.get(),
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
void LitParticlesRenderer::ParticleDirectionInit(
  const std::shared_ptr<ParticlePool<LitParticlesData> > & a_pPool) {
  //Direction VBO Initialization
  glGenBuffers(1, &m_directionVBOID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticleColorsInit -> Generated direction VBO ID = %d\n", m_directionVBOID);
  glBindBuffer(GL_ARRAY_BUFFER, m_directionVBOID);
  ImGuiLog::GetInstance().AddLog("TextureCoreGLRenderer::ParticleColorsInit -> Allocated buffer memory for ID = %d\n", m_directionVBOID);

  const std::size_t wParticleCount = a_pPool->GetParticleCount();

  glBufferData(GL_ARRAY_BUFFER,
    sizeof(glm::f32vec3)*wParticleCount,
    a_pPool->pData->m_velocity.get(),
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

void LitParticlesRenderer::ParticleTexturesInit(const std::string& a_sTexturePath) {
  m_textureID = texture::factory::Create2DTexture(a_sTexturePath);
  shader::module::GetUniformLocation(m_shaderProgram, "mytexture");
}

void LitParticlesRenderer::Update() {
  shader::module::Use(m_shaderProgram);
  const std::size_t wActiveParticleCount =
    m_pPool->GetActiveParticleCount();

  if (wActiveParticleCount > 0) {
    glm::f32vec3 *positions = m_pPool->pData->m_position.get();
    glm::u8vec4 *colors = m_pPool->pData->m_color.get();
    glm::f32vec3 *velocities = m_pPool->pData->m_velocity.get();

    // TODO : Sort particles by distance to camera
    //for (int i = 0; i < wActiveParticleCount; ++i) {


    //}

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 
      sizeof(glm::f32vec3)*wActiveParticleCount, positions);

    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBOID);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
      sizeof(glm::u8vec4)*wActiveParticleCount, colors);

    glBindBuffer(GL_ARRAY_BUFFER, m_directionVBOID);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
      sizeof(glm::f32vec3)*wActiveParticleCount, velocities);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}
void LitParticlesRenderer::Render() {
  shader::module::Use(m_shaderProgram);
  glEnable(GL_BLEND);
  glDepthMask(false);
  //glBlendFunc(GL_SRC_COLOR, GL_ONE); // Useful for White on Black textures
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

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
  glDepthMask(true);
  glDisable(GL_BLEND);
}
} /* namespace lit_particles_project */
} /* namespace particle */
} /* namespace gem */
