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
#ifndef BASIC_STREAM_SOURCE_HH
#define BASIC_STREAM_SOURCE_HH

#include "source.hh"

namespace gem {
namespace particle {
class BasicStreamSource : public Source {
public:
  BasicStreamSource();
  BasicStreamSource(
    const glm::f32vec3& a_spawnLocation,
    const glm::f32vec3& a_spawnVelocity,
    float a_fLifetime,
    double a_dEmissionRate);
	virtual ~BasicStreamSource() = default;

  // TODO: Copyable and moveable?<

private:
  virtual void Init(double a_dt, const std::unique_ptr<Pool>& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) override;
}; /* class BasicStreamSource*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: BASIC_SOURCE_HH */
