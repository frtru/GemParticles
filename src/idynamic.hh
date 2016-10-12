#ifndef DYNAMIC_HH
#define DYNAMIC_HH

#include <memory>

#include "particle_pool.hh"

namespace Gem {
namespace Particle {
class IDynamic {
public:
	IDynamic() = default;
	virtual ~IDynamic() = default;

	// TODO: Copyable and moveable?<

  virtual void Update(double a_dt, const std::unique_ptr<Pool>& a_pPool) = 0;

private:

} /* class Dynamic*/;
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: DYNAMIC_HH */
