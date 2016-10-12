#include "fountain.hh"

#include <algorithm>

namespace Gem {
namespace Particle {
Fountain::Fountain(const glm::f32vec3& a_spawnLocation) 
  : ISource(0.0), // TODO: Fetch emission rate externally (parameter)
    m_spawnLocation(a_spawnLocation) {
  // TODO: Every parameter except for the acceleration
  // should be received from external party
}

Fountain::~Fountain() {}

void Fountain::Init(double a_dt, const std::unique_ptr<Pool>& a_pPool,
  std::size_t a_unStartID, std::size_t a_unEndID) {
  for (std::size_t i = a_unStartID; i < a_unEndID; ++i) {
    // TODO: See if += is possible and if 
    // an affectation like that overwrite/overlap with 
    // other sources work
    a_pPool->m_acceleration[i]  = GRAVITY_ACCEL;
    a_pPool->m_velocity[i]      = { 0.0f,0.0f,0.0f };
    a_pPool->m_position[i]      = m_spawnLocation;
    a_pPool->m_lifetime[i]      = 0.0f; //TODO: temporary, replace with m_lifetime;
    a_pPool->m_color[i]         = { 0.0f,0.0f,0.0f,0.0f }; //TODO: temporary, replace with m_color
  }
}
} /* namespace Particle */
} /* namespace Gem */
