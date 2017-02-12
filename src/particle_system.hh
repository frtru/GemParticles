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
#ifndef PARTICLE_SYSTEM_HH
#define PARTICLE_SYSTEM_HH

#include "particle_system_component.hh"
#include "renderer.hh"
#include "macro_definitions.hh"

namespace Gem {
namespace Particle {
// TODO: Could maybe try to have template metaprogrammed
// Component and Renderer?
class ParticleSystem {
  DECLARE_DEFAULT_COPYABLE(ParticleSystem)
  DECLARE_DEFAULT_MOVABLE(ParticleSystem)
public:
  ParticleSystem(
    ParticleSystemComponent *a_pComponent,
    Renderer *a_pRenderer);
  ~ParticleSystem();

  void Link();
  void Update(double a_dt);
  void Render();

private:
  ParticleSystemComponent *m_pComponent;
  Renderer                *m_pRenderer;
}; /* class ParticleSystem */
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: PARTICLE_SYSTEM_HH */
