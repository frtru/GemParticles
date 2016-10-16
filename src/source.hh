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
#ifndef SOURCE_HH
#define SOURCE_HH

#include <glm/glm.hpp>

#include "particle_pool.hh"

namespace Gem {
namespace Particle {
class Source {
public:
	Source() = delete;
	Source(const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime,
    double a_dEmissionRate);
	~Source() = default;

  /*TODO: When you'll be at the point to set the rate through
  the UI, it will be the time start caring about those things
  double GetEmissionRate() const { return m_dEmissionRate; }
  void SetEmissionRate(double a_dEmissionRate) {
    m_dEmissionRate = a_dEmissionRate;
  }*/

  // TODO: Copyable and moveable?<
  void Emit(double a_dt, const std::unique_ptr<Pool>& a_pPool);

protected:
  float         m_fLifetime;
  double        m_dEmissionRate;
  glm::f32vec3  m_spawnLocation;
  glm::f32vec3  m_spawnVelocity;

private:
  // Private initialization of the particles before emission
  // in the subclasses
  virtual void Init(double a_dt, const std::unique_ptr<Pool>& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) = 0;
} /* class Source*/;
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: SOURCE_HH */
