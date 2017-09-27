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
#ifndef DYNAMIC_HH
#define DYNAMIC_HH

#include <memory>

#include "particle_pool.hh"
#include "macro_definitions.hh"

namespace gem {
namespace particle {
template <class ParticleType>
class Dynamic {
  DECLARE_UNCOPYABLE(Dynamic)
  DECLARE_UNMOVABLE(Dynamic)
public:
	Dynamic() = default;
	virtual ~Dynamic() = default;

  virtual void Update(double a_dt, const std::shared_ptr<ParticlePool<ParticleType> >& a_pPool) = 0;
  
  virtual bool AltersParticleLifeCycle() const { return false; }
}; /* class Dynamic*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: DYNAMIC_HH */
