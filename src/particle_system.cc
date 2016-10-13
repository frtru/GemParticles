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
#include "particle_system.hh"

namespace Gem {
namespace Particle {
System::System(std::size_t a_unMaxParticleCount) 
  : m_pParticlePool (new Pool(a_unMaxParticleCount)) {
}

System::~System() {}

void System::Udpate(double a_dt){
  for (IDynamic* dyn : m_vDynamics) {
    dyn->Update(a_dt,m_pParticlePool);
  }
  //TODO: Update the sources as welll
}
} /* namespace Particle */
} /* namespace Gem */

