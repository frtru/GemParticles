#ifndef FOUNTAIN_HH
#define FOUNTAIN_HH

#include "isource.hh"

namespace Gem {
namespace Particle {
class Fountain : public ISource {
private:
  // TODO: When you have extra time,
  // put the following in a file  
  // dedicated to math constants
  // (Gem::Utils::MathConstants or something like
  // that) or find a library (if it isn't
  // already defined by C++ standards)
  // that defines it...
  const glm::f32vec3 GRAVITY_ACCEL = { 0,9.80665,0 };
public:
	Fountain() = delete;
	Fountain(const glm::f32vec3& a_spawnLocation);
	~Fountain();

  // TODO: Copyable and moveable?<

private:
  virtual void Init(double a_dt, const std::unique_ptr<Pool>& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) override;

  glm::f32vec3  m_spawnLocation;
} /* class Fountain*/;
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: FOUNTAIN_HH */
