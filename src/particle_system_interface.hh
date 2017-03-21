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
#ifndef PARTICLE_SYSTEM_INTERFACE_HH
#define PARTICLE_SYSTEM_INTERFACE_HH

#include <memory>
#include <string>

#include "macro_definitions.hh"

namespace gem {
namespace particle {
class IParticleSystem {
  DECLARE_UNCOPYABLE(IParticleSystem)
  DECLARE_MOVABLE(IParticleSystem)
public:
  IParticleSystem() = default;
  virtual ~IParticleSystem() = default;
  
  virtual inline std::size_t GetProgramID() const = 0;
  virtual inline std::size_t GetActiveParticlesCount() const = 0;

  virtual void Update(double a_dt) = 0;
  virtual void Render() = 0;
}; /* class IParticleSystem */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: PARTICLE_SYSTEM_INTERFACE_HH */
