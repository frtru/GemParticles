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

namespace gem {
namespace particle {
class Dynamic {
public:
	Dynamic() = default;
	virtual ~Dynamic() = default;

	// TODO: Copyable and moveable?<

  virtual void Update(double a_dt, const std::unique_ptr<Pool>& a_pPool) = 0;

private:

}; /* class Dynamic*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: DYNAMIC_HH */
