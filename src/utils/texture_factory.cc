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
#include "utils/texture_factory.hh"

#include <mutex>

#include "FreeImage.h"
#include "utils/imgui/imgui_log.h"

namespace texture {
namespace factory {
namespace {
std::once_flag init_flag;
std::once_flag terminate_flag;

FIBITMAP       *bitmap_handle; //pointer to the image, once loaded
/*  
 * LoadFunction taken from documentation/examples provided by
 * FreeImage. Fixed some issues like 32 bits conversion and
 * image handles not being properly shipped out of the function.
 */
bool LoadImage(const std::string& a_sFileName,
  unsigned char **a_pImage,
  GLsizei& a_rWidth,
  GLsizei& a_rHeight,
  bool a_bImageHasAlphaChannel) {
  const char *wFilename = a_sFileName.c_str();
  bitmap_handle = nullptr;
  //image format
  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

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
    bitmap_handle = a_bImageHasAlphaChannel ?
      FreeImage_ConvertTo32Bits(FreeImage_Load(fif, wFilename)) :
      FreeImage_Load(fif, wFilename);
  //if the image failed to load, return failure
  if (!bitmap_handle)
    return false;

  //retrieve the image data
  bits = FreeImage_GetBits(bitmap_handle);
  width = FreeImage_GetWidth(bitmap_handle);
  height = FreeImage_GetHeight(bitmap_handle);
  //if this somehow failed (it shouldn't), return failure
  if ((bits == 0) || (width == 0) || (height == 0))
    return false;
  
  *a_pImage = bits;
  a_rWidth = width;
  a_rHeight = height;

  return true;
}

bool FreeImage() {
  FreeImage_Unload(bitmap_handle);
  bitmap_handle = nullptr;
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
  
  if (!LoadImage(a_sFileName, &wImage, wWidth, wHeight, a_nInternalFormat == GL_RGBA)) {
    ImGuiLog::GetInstance().AddLog("[ERROR]texture_factory::Create2DTexture -> Loading image failed.\n Returning 0xFFFFFFFF...\n");
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

  FreeImage();
  glBindTexture(GL_TEXTURE_2D, 0);

  return wTexture;
}

GLuint CreateTextureFromMemoryPixels(unsigned char *pixels, GLint width, GLint height) {
  GLuint wTexture;
  glGenTextures(1, &wTexture);
  glBindTexture(GL_TEXTURE_2D, wTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(
    GL_TEXTURE_2D,      // 2D texture target
    0,                  // Base mipmap level
    GL_RGBA,            // RGB color components
    width, height,      // Dimensions
    0,                  // Must be 0...
    GL_RGBA,            // Pixel data format
    GL_UNSIGNED_BYTE,   // Depends on what the LoadImage function return type
    pixels);            // Loaded image
  return wTexture;
}

GLuint CreateCubeMap(const std::vector<std::string>& a_vTextures,
 GLint a_nMagFilterParam, GLint a_nMinFilterParm,
 GLint a_nTexHorizontalWrapParam, GLint a_nTexVerticalWrapParam, GLint a_nTexDepthWrapParam,
 GLint a_nInternalFormat, GLint a_nImageFormat) {
  GLuint wTexture;
  glGenTextures(1, &wTexture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, wTexture);
  
  for (std::size_t i = 0; i < a_vTextures.size(); ++i) {
    std::string textureFileName = a_vTextures[i];
    GLsizei wWidth, wHeight;
    unsigned char *wImage = nullptr;

    if (!LoadImage(textureFileName, &wImage, wWidth, wHeight, a_nInternalFormat == GL_RGBA)) {
      ImGuiLog::GetInstance().AddLog("[ERROR]texture_factory::CreateCubeMap -> Loading image %s failed.\n Returning 0xFFFFFFFF...\n", textureFileName.c_str());
      return 0xFFFFFFFF;
    }
    glTexImage2D(
      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,      // 2D texture target
      0,                  // Base mipmap level
      a_nInternalFormat,  // RGB color components
      wWidth, wHeight,    // Dimensions
      0,                  // Must be 0...
      a_nImageFormat,     // Pixel data format
      GL_UNSIGNED_BYTE,   // Depends on what the LoadImage function return type
      wImage);            // Loaded image

    FreeImage();
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, a_nMagFilterParam);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, a_nMinFilterParm);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, a_nTexHorizontalWrapParam);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, a_nTexVerticalWrapParam);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, a_nTexDepthWrapParam);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  return wTexture;
}
} /* namespace factory */
} /* namespace texture */