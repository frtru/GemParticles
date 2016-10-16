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
#include "particle_system.hh"

#include "default_dynamic.hh"

namespace Gem {
namespace Particle {
System::System(std::size_t a_unMaxParticleCount) 
  : m_pParticlePool(new Pool(a_unMaxParticleCount)) {
  m_vDynamics.push_back(std::make_unique<DefaultDynamic>());
}
System::System(System&& other)
  : m_pParticlePool(std::move(other.m_pParticlePool)),
  m_vDynamics(std::move(other.m_vDynamics)),
  m_vSources(std::move(other.m_vSources)) {
}
void System::Update(double a_dt){
  for (auto& source : m_vSources) {
    source->Emit(a_dt, m_pParticlePool);
  }
  for (auto& dynamic : m_vDynamics) {
    dynamic->Update(a_dt, m_pParticlePool);
  }
}
} /* namespace Particle */
} /* namespace Gem */

