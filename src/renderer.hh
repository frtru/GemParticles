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
#ifndef RENDERER_HH
#define RENDERER_HH

#include <memory>

#include "particle_pool.hh"

// TODO: Find a way like fenbf did for the billboard,
// glpoint, bool useQuads thingy...

namespace gem {
namespace particle {
class Renderer {
public:
	Renderer() = default;
	virtual ~Renderer() = default;

  // TODO: See if init/terminate should be changed for 
  // something more stable, like object construction/destruction
  // TODO: Remove the PUBLIC init/terminate idiom in classes and put initialization in constructor.

  virtual void Init(Pool* a_pPool) = 0;
  virtual void Terminate() = 0;
  virtual void Update() = 0;
  virtual void Render() = 0;
  
protected:
  // TODO: If pool is better with managed pointers, change them to shared and here also
  Pool* m_pParticlePool;

}; /* class Renderer*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: RENDERER_HH */
