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
#ifndef OPENGL_CONTEXT_HH
#define OPENGL_CONTEXT_HH

#include <memory>

#include <gl/glew.h>
#include <gl/glfw3.h>

#include "graphic_context.hh"
//#include "shader.hh"

// TODO: Should the shadermanager initialization be 
// done by this class?

namespace Gem {
namespace Particle {
class OpenGLContext : public GraphicContext {
public:
  OpenGLContext();
  virtual ~OpenGLContext();

  virtual void Update() override;
  virtual bool PollWindowClosedEvent() override;
  virtual void Reshape(int a_width, int a_height) override;

  virtual std::shared_ptr<void> GetWindowHandle() const override;

private:
  virtual void InitImpl() override;
  virtual void TerminateImpl() override;

  std::shared_ptr<GLFWwindow> m_pWindow;

}; /* class OpenGLContext*/
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: OPENGL_CONTEXT_HH */

