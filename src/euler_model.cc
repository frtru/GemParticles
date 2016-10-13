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
#include "euler_model.hh"

namespace Gem {
namespace Particle {
EulerModel::EulerModel(){
}

EulerModel::~EulerModel(){
}

void EulerModel::Update(double a_dt, const std::unique_ptr<Pool>& a_pPool) {
  // TODO: Deal with the delta double precision casted to float later
  const float fDt = (float)a_dt;
  for (int i = 0; i < a_pPool->GetActiveParticleCount(); ++i) {
    a_pPool->m_position[i] += a_pPool->m_velocity[i] * fDt;
    a_pPool->m_velocity[i] += a_pPool->m_acceleration[i] * fDt;
  }
}
} /* namespace Particle */
} /* namespace Gem */
