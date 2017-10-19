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
#ifndef ATTRACTOR_EVENT_HANDLER_HH
#define ATTRACTOR_EVENT_HANDLER_HH

#include <memory>
#include "graphic_contexts/graphic_context.hh"
#include "projects/attractors/proximity_color_updater.hh"
#include "dynamics/particle_attractor.hh"

namespace gem { namespace particle {
namespace attractor_project {
namespace event_handler {
void Init(const std::shared_ptr<GraphicContext>& a_pCtxt, 
  const std::shared_ptr<ParticleAttractor>& a_pAttractorHandle,
  const std::shared_ptr<ProximityColorUpdater>& a_pColorUpdater);
void Terminate();
void Update();
} /* namespace event_handler*/
} /* namespace attractor_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: ATTRACTOR_EVENT_HANDLER_HH */

