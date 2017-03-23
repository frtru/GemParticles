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

#include <iostream>
#include <mutex>

#include "FreeImage.h"

namespace texture {
namespace factory {
namespace {
std::once_flag init_flag;
std::once_flag terminate_flag;

// TODO: I don't really like the bitmaphandle
// in order to free it passed as parameter...
bool LoadImage(const std::string& a_sFileName,
  unsigned char *a_pImage,
  std::size_t& a_rWidth, 
  std::size_t& a_rHeight,
  void *a_pBitmapHandle) {
  const char *wFilename = a_sFileName.c_str();
  //image format
  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
  //pointer to the image, once loaded
  FIBITMAP *dib(0);
  //pointer to the image data
  BYTE* bits(0);
  //image width and height
  unsigned int width(0), height(0);
  //OpenGL's image ID to map to
  GLuint gl_texID;

  //check the file signature and deduce its format
  fif = FreeImage_GetFileType(wFilename, 0);
  //if still unknown, try to guess the file format from the file extension
  if (fif == FIF_UNKNOWN)
    fif = FreeImage_GetFIFFromFilename(wFilename);
  //if still unkown, return failure
  if (fif == FIF_UNKNOWN)
    return false;

  //check that the plugin has reading capabilities and load the file
  if (FreeImage_FIFSupportsReading(fif))
    dib = FreeImage_Load(fif, wFilename);
  //if the image failed to load, return failure
  if (!dib)
    return false;

  //retrieve the image data
  bits = FreeImage_GetBits(dib);
  //get the image width and height
  width = FreeImage_GetWidth(dib);
  height = FreeImage_GetHeight(dib);
  //if this somehow one of these failed (they shouldn't), return failure
  if ((bits == 0) || (width == 0) || (height == 0))
    return false;
  
  a_pImage = bits;
  a_rWidth = width;
  a_rHeight = height;
  a_pBitmapHandle = dib;

  return true;
}

bool FreeImage(void *a_pBitmapHandle) {
  FreeImage_Unload(static_cast<FIBITMAP *>(a_pBitmapHandle));
  return true;
}
}

void Init() {
  std::call_once(init_flag, [&]() {
#ifdef FREEIMAGE_LIB
    FreeImage_Initialise();
#endif
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
#ifdef FREEIMAGE_LIB
    FreeImage_DeInitialise();
#endif
  });
}

// TODO: Pass some of the parameters of glTexImage2D
// from here
bool Create2DTexture(const std::string& a_sFileName) {
  // Load Image
  std::size_t wWidth, wHeight;
  unsigned char *wImage = nullptr;
  void *wBitMapHandle = nullptr;
  if (!LoadImage(a_sFileName, wImage, wWidth, wHeight, wBitMapHandle)) {
    return false;
  }
  
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
  FreeImage(wBitMapHandle);
  glBindTexture(GL_TEXTURE_2D, 0);

  return true;
}
} /* namespace factory */
} /* namespace texture */