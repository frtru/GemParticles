/*************************************************************************
 * Copyright (c) 2016 Fran�ois Trudel
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
#ifndef SCENE_HH
#define SCENE_HH

#include "utils/light_module.hh"

namespace gem { namespace particle {
namespace light_project {
namespace scene {
void Init(bool a_isDebug = false);
void Terminate();

void UpdateMaterial(const light::Material& a_material);
light::Material GetMaterial();
bool IsDebug();
void SetDebugOption(bool a_isDebug);
void Render();
} /* namespace scene */
} /* namespace light_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: SCENE_HH */

