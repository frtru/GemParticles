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

#include "particle_system.hh"

// TODO: Find a way like fenbf did for the billboard,
// glpoint, bool useQuads thingy...

namespace Gem {
namespace Particle {
class Renderer {
public:
	Renderer() = default;
	virtual ~Renderer() = default;

	// TODO: Copyable and moveable?<
  void Init(const System& a_system);
  virtual void Render() = 0;
  
private:

}; /* class Renderer*/
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: RENDERER_HH */
