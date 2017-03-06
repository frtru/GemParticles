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
#include "macro_definitions.hh"
// TODO: Find a way like fenbf did for the billboard,
// glpoint, bool useQuads thingy...

namespace gem {
namespace particle {
class Renderer {
  DECLARE_UNCOPYABLE(Renderer)
  DECLARE_UNMOVABLE(Renderer)
public:
	Renderer() = default;
	virtual ~Renderer() = default;

protected:
  virtual void Update(const std::shared_ptr<ParticlePool> &a_pPool) = 0;
  virtual void Render(const std::shared_ptr<ParticlePool> &a_pPool) = 0;
}; /* class Renderer*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: RENDERER_HH */
