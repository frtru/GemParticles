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
#ifndef OPENGL_RENDERER_HH
#define OPENGL_RENDERER_HH

#include <GL/glew.h>

#include "renderer.hh"

namespace gem {
namespace particle {
class GLRenderer : public Renderer {
public:
  GLRenderer(const std::shared_ptr<ParticlePool> &a_pPool);
  virtual ~GLRenderer();

  virtual void Update(const std::shared_ptr<ParticlePool> &a_pPool) = 0;
  virtual void Render(const std::shared_ptr<ParticlePool> &a_pPool) = 0;

protected:
  GLuint m_vertexArrayID;   //VAO
  GLuint m_vertexBufferID;  //VBO
}; /* class GLRenderer */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: OPENGL_RENDERER_HH */
