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
 *
 * Inspired by: http://www.bfilipek.com/2014/04/flexible-particle-system-start.html
*************************************************************************/

/* TODOs
 * 1- Why not just a const getter for the particle pool instead of public?
 * 2- Since we have the lifetime > 0.0 to see if particles are active,
 * an active particle counter and the pool property of having active particles
 * at the front, couldn't we just get rid of the activeflags container??
 * Maybe performance wise it is just safer to have one...TBD
 * 3- Try AoS vs SoA (in other words move the particle parameters here
 * and create an array for each
 * 4- See unique_ptr vs vector for pool
 */
#ifndef PARTICLE_SYSTEM_COMPONENT_HH
#define PARTICLE_SYSTEM_COMPONENT_HH

// TODO: Rename that to remove component from the name...
// TODO: Move system name into particlesystem...

#include <vector>
#include <memory>

#include "particle_pool.hh"
#include "emitter.hh"
#include "dynamic.hh"
#include "macro_definitions.hh"

namespace gem {
namespace particle {
  // TODO : If it possible to use template meta-programmed
  // in particle system, put the type of pool/particledata as template
class ParticleSystemComponent {
  DECLARE_UNCOPYABLE(ParticleSystemComponent)
  DECLARE_UNMOVABLE(ParticleSystemComponent)
public:
  explicit ParticleSystemComponent(
    std::size_t a_unMaxParticleCount);
  ~ParticleSystemComponent() = default;

  void Update(double a_dt);
  const std::shared_ptr<ParticlePool>& GetParticles() const {
    return m_pParticlePool; 
  }
  void AddEmitter(std::unique_ptr<Emitter> a_pEmitter) { 
    m_vEmitters.push_back(std::move(a_pEmitter)); 
  }
  void AddDynamic(std::unique_ptr<Dynamic> a_pDynamic) { 
    m_vDynamics.push_back(std::move(a_pDynamic)); 
  }

private:
  std::shared_ptr<ParticlePool>                   m_pParticlePool; 
  std::vector<std::unique_ptr<Emitter> >  m_vEmitters;
  std::vector<std::unique_ptr<Dynamic> >  m_vDynamics;
}; /* class ParticleSystemComponent */
} /* namespace particle */
} /* namespace gem */


#endif /* end of include guard: PARTICLE_SYSTEM_COMPONENT_HH */
