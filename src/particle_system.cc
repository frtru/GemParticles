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

