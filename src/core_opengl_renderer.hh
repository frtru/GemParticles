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
#ifndef CORE_GL_RENDERER_HH
#define CORE_GL_RENDERER_HH

#include <GL/glew.h>

#include "renderer.hh"
#include "particle_pool_core.hh"

namespace gem {
namespace particle {
class CoreGLRenderer : public Renderer<CoreParticles> {
public:
  CoreGLRenderer(const std::shared_ptr<ParticlePool<CoreParticles> > &a_pPool);
  virtual ~CoreGLRenderer();

  virtual std::size_t GetProgramID() const {
    return m_shaderProgram;
  }

  virtual void Update(const std::shared_ptr<ParticlePool<CoreParticles> > &a_pPool) override;
  virtual void Render(const std::shared_ptr<ParticlePool<CoreParticles> > &a_pPool) override;
private:
  void ParticlePositionsInit(const std::shared_ptr<ParticlePool<CoreParticles> > & a_pPool);
  void ParticleColorsInit(const std::shared_ptr<ParticlePool<CoreParticles> > & a_pPool);
  void ParticleTexturesInit();

  GLuint  m_vertexArrayID;
  GLuint  m_vertexBufferID;
  GLuint  m_colorVBOID;

  GLuint  m_shaderProgram;
  GLuint  m_textureID;
}; /* class CoreGLRenderer*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: CORE_GL_RENDERER_HH */
