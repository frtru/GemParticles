/*************************************************************************
 * Copyright (c) 2017 François Trudel
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

/*  
 * LoadFunction taken from documentation/examples provided by
 * FreeImage. Fixed some issues like 32 bits conversion and
 * image handles not being properly shipped out of the function.
 */
bool LoadImage(const std::string& a_sFileName,
  unsigned char **a_pImage,
  GLsizei& a_rWidth,
  GLsizei& a_rHeight,
  void **a_pBitmapHandle) {
  const char *wFilename = a_sFileName.c_str();
  //image format
  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
  //pointer to the image, once loaded
  FIBITMAP *dib(0);
  //pointer to the image data
  BYTE* bits(0);
  //image width and height
  unsigned int width(0), height(0);

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
    dib = FreeImage_ConvertTo32Bits(FreeImage_Load(fif, wFilename));
  //if the image failed to load, return failure
  if (!dib)
    return false;

  //retrieve the image data
  bits = FreeImage_GetBits(dib);
  width = FreeImage_GetWidth(dib);
  height = FreeImage_GetHeight(dib);
  //if this somehow failed (it shouldn't), return failure
  if ((bits == 0) || (width == 0) || (height == 0))
    return false;
  
  *a_pImage = bits;
  *a_pBitmapHandle = dib;
  a_rWidth = width;
  a_rHeight = height;

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

GLuint Create2DTexture(const std::string& a_sFileName,
  GLint a_nMagFilterParam, GLint a_nMinFilterParm,
  GLint a_nTexHorizontalWrapParam, GLint a_nTexVerticalWrapParam,
  GLint a_nInternalFormat, GLint a_nImageFormat) {
  GLsizei wWidth, wHeight;
  unsigned char *wImage = nullptr;
  void *wBitMapHandle = nullptr;
  
  if (!LoadImage(a_sFileName, &wImage, wWidth, wHeight, &wBitMapHandle)) {
    std::cerr << "texture_factory::Create2DTexture -> Loading image failed. " << std::endl
      << "Returning 0xFFFFFFFF..." << std::endl;
    return 0xFFFFFFFF;
  }

  GLuint wTexture;
  glGenTextures(1, &wTexture);
  glBindTexture(GL_TEXTURE_2D, wTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, a_nMinFilterParm);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, a_nMagFilterParam);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, a_nTexHorizontalWrapParam);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, a_nTexVerticalWrapParam);

  glTexImage2D(
    GL_TEXTURE_2D,      // 2D texture target
    0,                  // Base mipmap level
    a_nInternalFormat,  // RGB color components
    wWidth, wHeight,    // Dimensions
    0,                  // Must be 0...
    a_nImageFormat,     // Pixel data format
    GL_UNSIGNED_BYTE,   // Depends on what the LoadImage function return type
    wImage);            // Loaded image
    glGenerateMipmap(GL_TEXTURE_2D);

  FreeImage(wBitMapHandle);
  glBindTexture(GL_TEXTURE_2D, 0);

  return wTexture;
}
} /* namespace factory */
} /* namespace texture */