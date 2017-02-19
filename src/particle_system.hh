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

#include <memory>

#include "particle_system_component.hh"
#include "renderer.hh"
#include "macro_definitions.hh"

namespace gem {
namespace particle {
// TODO: Could maybe try to have template metaprogrammed
// Component and Renderer?
class ParticleSystem {
  DECLARE_UNCOPYABLE(ParticleSystem)
  DECLARE_MOVABLE(ParticleSystem)
public:
  explicit ParticleSystem(
    std::unique_ptr<ParticleSystemComponent> &&a_pComponent,
    std::unique_ptr<Renderer> &&a_pRenderer,
    std::string &&a_sSystemName = std::move(std::string("DEFAULT_SYS_NAME")));
  ~ParticleSystem();

  // TODO: Remove the PUBLIC init/terminate idiom in classes and put initialization in constructor.
  void Init();
  void Terminate();

  void Update(double a_dt);
  void Render();

private:
  std::string                               m_sSystemName;
  std::unique_ptr<ParticleSystemComponent>  m_pComponent;
  std::unique_ptr<Renderer>                 m_pRenderer;
}; /* class ParticleSystem */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: PARTICLE_SYSTEM_HH */
