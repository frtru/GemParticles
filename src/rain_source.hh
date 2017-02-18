/*************************************************************************
 * Copyright (c) 2016 Fran�ois Trudel
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
#ifndef RAIN_SOURCE_HH
#define RAIN_SOURCE_HH

#include "source.hh"

namespace gem {
namespace particle {
class RainSource : public Source {
public:
  RainSource() = default; // Can be defaulted to base default constructor
  RainSource(float a_fLifetime, double a_dEmissionRate);
	~RainSource() = default;

  // TODO: Copyable and moveable?<

private:
  virtual void Init(double a_dt, const std::unique_ptr<Pool>& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) override;
}; /* class RandomFountainSource*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: RAIN_SOURCE_HH */
