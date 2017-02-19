/*************************************************************************
 * Copyright (c) 2016 Fran�ois Trudel
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
#ifndef SIMPLE_GL_RENDERER_HH
#define SIMPLE_GL_RENDERER_HH

#include "opengl_renderer.hh"

namespace gem {
namespace particle {
// TODO: Rename this for something different than stub...
// something like basic or debug
class SimpleGLRenderer : public GLRenderer {
public:
  SimpleGLRenderer();
  virtual ~SimpleGLRenderer() = default;

  virtual void Update() override;
  virtual void Render() override;

private:
  virtual void InitImpl() override;
  virtual void TerminateImpl() override;

  // An extra buffer for color
  GLuint  m_colorVBOID;

}; /* class SimpleGLRenderer*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: SIMPLE_GL_RENDERER_HH */