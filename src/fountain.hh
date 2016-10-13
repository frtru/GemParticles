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
#ifndef FOUNTAIN_HH
#define FOUNTAIN_HH

#include "isource.hh"

namespace Gem {
namespace Particle {
class Fountain : public ISource {
private:
  // TODO: When you have extra time,
  // put the following in a file  
  // dedicated to math constants
  // (Gem::Utils::MathConstants or something like
  // that) or find a library (if it isn't
  // already defined by C++ standards)
  // that defines it...
  const glm::f32vec3 GRAVITY_ACCEL = { 0,9.80665,0 };
public:
	Fountain() = delete;
  // TODO: Maybe the start/end color in the pool directly
  // is not necessary if we set it here,
  // or maybe in the updaters...
  Fountain(const glm::f32vec3& a_spawnLocation,
    /*const glm::f32vec3& a_spawnVelocity,
    const glm::f32vec3& a_spawnAcceleration,
    const glm::u8vec4& a_startColor,
    const glm::u8vec4& a_EndColor,
    float lifetime = 1.0f,*/
    double a_dEmissionRate = 0.0);
	~Fountain();

  // TODO: Copyable and moveable?<

private:
  virtual void Init(double a_dt, const std::unique_ptr<Pool>& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) override;

  glm::f32vec3  m_spawnLocation;
} /* class Fountain*/;
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: FOUNTAIN_HH */
