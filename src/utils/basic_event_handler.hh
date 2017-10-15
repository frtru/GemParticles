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
#ifndef EVENT_HANDLER_HH
#define EVENT_HANDLER_HH

#include <memory>

namespace gem {
namespace particle {
class GraphicContext;

namespace event_handler {
void Init(const std::shared_ptr<GraphicContext>& a_pCtxt);
void Terminate();
}; /* namespace event_handler*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: EVENT_HANDLER_HH */

