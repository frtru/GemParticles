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
#ifndef FOUNTAIN_SOURCE_HH
#define FOUNTAIN_SOURCE_HH

#include "source.hh"

namespace Gem {
namespace Particle {
class FountainSource : public Source {
private:
  static const glm::f32vec3 DEFAULT_SPEED;

public:
	FountainSource() = delete;
  FountainSource(const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity = DEFAULT_SPEED,
    float a_fLifetime = 1.0f,
    double a_dEmissionRate = 100.0);
	~FountainSource() = default;

  // TODO: Copyable and moveable?<

private:
  virtual void Init(double a_dt, const std::unique_ptr<Pool>& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) override;
}; /* class FountainSource*/
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: FOUNTAIN_SOURCE_HH */
