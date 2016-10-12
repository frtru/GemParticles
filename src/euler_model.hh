#ifndef EULER_GRAVITY_MODEL_HH
#define EULER_GRAVITY_MODEL_HH

#include "idynamic.hh"

namespace Gem {
namespace Particle {
class EulerModel : public IDynamic {
public:
  EulerModel();
	~EulerModel();

	// TODO: Copyable and moveable?<

  virtual void Update(double a_dt, const std::unique_ptr<Pool>& a_pPool) override;
} /* class Dynamic*/;
} /* namespace Particle */
} /* namespace Gem */

#endif /* end of include guard: EULER_GRAVITY_MODEL_HH */
