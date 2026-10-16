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
#ifndef RAIN_PROJECT_HH
#define RAIN_PROJECT_HH

#include <memory>

namespace gem { namespace particle {
class GraphicContext;

namespace rain_project {
void Init(const std::shared_ptr<GraphicContext>& a_pContext);
void Update(double a_dt);
void Terminate();
} /* namespace rain_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: RAIN_PROJECT_HH */
