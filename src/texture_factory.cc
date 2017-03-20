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
#include "texture_factory.hh"

#include <mutex>

namespace texture {
namespace factory {
namespace {
std::once_flag init_flag;
std::once_flag terminate_flag;

unsigned char *LoadImage(const std::string& a_sFileName,
  std::size_t& a_rWidth, std::size_t& a_rHeight) {
  // TODO:
  return NULL;
}

void FreeImage(unsigned char *a_pImage) {

}
}

void Init() {
  std::call_once(init_flag, [&]() {
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
  });
}

// TODO: Pass some of the parameters of glTexImage2D
// from here
bool Create2DTexture(const std::string& a_sFileName) {
  // Load Image
  std::size_t wWidth, wHeight;
  unsigned char *wImage =
    LoadImage(a_sFileName,wWidth, wHeight);
  
  // Generate texture
  GLuint wTexture;
  glGenTextures(1, &wTexture);

  // Create texture
  glBindTexture(GL_TEXTURE_2D, wTexture);
  glTexImage2D(
    GL_TEXTURE_2D,    // 2D texture target
    0,                // Base mipmap level
    GL_RGB,           // RGB color components
    wWidth, wHeight,  // Dimensions
    0,                // Must be 0...
    GL_RGB,           // Pixel data format
    GL_UNSIGNED_BYTE, // Depends on what the LoadImage function return type
    wImage);          // Loaded image
  glGenerateMipmap(GL_TEXTURE_2D);

  // Free image
  FreeImage(wImage);
  glBindTexture(GL_TEXTURE_2D, 0);

  return true;
}
} /* namespace factory */
} /* namespace texture */