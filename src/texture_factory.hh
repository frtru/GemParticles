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
#ifndef TEXTURE_FACTORY_HH
#define TEXTURE_FACTORY_HH

#include <GL/glew.h>
#include <string>

namespace texture {
namespace factory {
void Init();
void Terminate();

GLuint Create2DTexture(const std::string& a_sFileName,
  GLint a_nMagFilterParam = GL_LINEAR, 
  GLint a_nMinFilterParm = GL_LINEAR,
  GLint a_nTexHorizontalWrapParam = GL_REPEAT, 
  GLint a_nTexVerticalWrapParam = GL_REPEAT,
  GLint a_nInternalFormat = GL_RGBA8, 
  GLint a_nImageFormat = GL_BGRA);
} /* namespace factory */
} /* namespace texture */
#endif /* end of include guard: TEXTURE_FACTORY_HH */
