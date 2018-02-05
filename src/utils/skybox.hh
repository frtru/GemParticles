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
#ifndef SKYBOX_HH
#define SKYBOX_HH

#include <string>
#include <vector>

namespace gem {
namespace particle {
namespace skybox {
void LoadSkyBox();
void Destroy();
// Can be used BEFORE calling LoadSkyBox to override
// the textures used for the skybox.
// NOTE: The textures must be provided in the following
// order: right, left, top, bottom, back, front.
void SetTextures(std::vector<std::string>&& a_vTextures);
void Render();
} /* namespace skybox */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: SKYBOX_HH */

