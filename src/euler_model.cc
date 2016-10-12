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
