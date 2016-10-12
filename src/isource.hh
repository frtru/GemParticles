#ifndef SOURCE_HH
#define SOURCE_HH

#include <glm/glm.hpp>

#include "particle_pool.hh"

namespace Gem {
namespace Particle {
class ISource {
public:
	ISource() = delete;
	ISource(double a_dEmissionRate = 0.0);
	~ISource();

  // TODO: Copyable and moveable?<
  void Emit(double a_dt, const std::unique_ptr<Pool>& a_pPool);
private:
  // Private initialization of the particles before emission
  // in the subclasses
  virtual void Init(double a_dt, const std::unique_ptr<Pool>& a_pPool,
    std::size_t a_unStartID, std::size_t a_unEndID) = 0;

  // TODO: Set this correctly
  double  m_dEmissionRate; // should this really be private? get/set?
} /* class Source*/;
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: SOURCE_HH */
