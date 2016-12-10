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
#ifndef STUB_RENDERER_HH
#define STUB_RENDERER_HH

#include "renderer.hh"

namespace Gem {
namespace Particle {
class StubRenderer : public Renderer {
public:
  StubRenderer();
  ~StubRenderer();

	// TODO: Copyable and moveable?<
  virtual void Render(std::unique_ptr<Pool> a_pParticlePool) override;

private:
  // TODO: See if the following could be refactored in the base class
  GLuint m_vertexArrayID;   //VAO
  GLuint m_vertexBufferID;  //VBO


}; /* class StubRenderer*/
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: STUB_RENDERER_HH */
