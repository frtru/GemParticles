#include "isource.hh"

#include <algorithm>

namespace Gem {
namespace Particle {
ISource::ISource(double a_dEmissionRate)
  : m_dEmissionRate(a_dEmissionRate) {}

ISource::~ISource() {}

void ISource::Emit(double a_dt, const std::unique_ptr<Pool>& a_pPool) {
  const std::size_t maxNewParticles = 
    static_cast<std::size_t>(a_dt*m_dEmissionRate);
  const std::size_t firstParticleIndex = 
    a_pPool->GetActiveParticleCount();
  const std::size_t lastParticleIndex = std::min(
    firstParticleIndex + maxNewParticles, 
    a_pPool->GetParticleCount() - 1);

  Init(a_dt, a_pPool, firstParticleIndex, lastParticleIndex);

  for (std::size_t i = firstParticleIndex; i < lastParticleIndex; ++i) {
    a_pPool->Wake(i);
  }
}
} /* namespace Particle */
} /* namespace Gem */
