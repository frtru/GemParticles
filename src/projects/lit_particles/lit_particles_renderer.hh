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
#ifndef LIT_PARTICLES_RENDERER_HH
#define LIT_PARTICLES_RENDERER_HH

#include <GL/glew.h>

#include "renderers/renderer.hh"
#include "projects/lit_particles/lit_particles_pool.hh"

namespace gem { namespace particle {
namespace lit_particles_project {
class LitParticlesRenderer : public Renderer<LitParticlesData> {
public:
  LitParticlesRenderer(const std::string& a_sTexturePath, float a_fParticleSize = 1.0f);
  virtual ~LitParticlesRenderer();

  inline std::size_t GetProgramID() const override {
    return m_shaderProgram;
  }
  
  virtual void Bind(const std::shared_ptr<ParticlePool<LitParticlesData> > &a_pPool) override;
  virtual void Update() override;
  virtual void Render() override;
private:
  void ParticlePositionsInit(const std::shared_ptr<ParticlePool<LitParticlesData> > & a_pPool);
  void ParticleColorsInit(const std::shared_ptr<ParticlePool<LitParticlesData> > & a_pPool);
  void ParticleDirectionInit(const std::shared_ptr<ParticlePool<LitParticlesData> > & a_pPool);
  void ParticleTexturesInit(const std::string& a_sTexturePath);

  GLuint  m_vertexArrayID;
  GLuint  m_vertexBufferID;
  GLuint  m_colorVBOID;
  GLuint  m_directionVBOID;

  GLuint  m_shaderProgram;
  GLuint  m_textureID;
  float   m_fParticleSize;
}; /* class LitParticlesRenderer*/
} /* lit_particles_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: LIT_PARTICLES_RENDERER_HH */
