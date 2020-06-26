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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "utils/imgui/imgui_log.h"

namespace texture {
namespace factory {
namespace {
std::once_flag init_flag;
std::once_flag terminate_flag;

bool LoadImage(const std::string& a_sFileName,
  unsigned char** a_pImage,
  GLsizei& a_rWidth,
  GLsizei& a_rHeight,
  GLsizei& a_rNbrChannels) {
  *a_pImage = stbi_load(a_sFileName.c_str(), &a_rWidth, &a_rHeight, &a_rNbrChannels, 0);
  return *a_pImage != nullptr;
}

void FreeImage(unsigned char* a_pImage) {
  stbi_image_free(a_pImage);
}
}

void Init() {
  std::call_once(init_flag, [&]() {
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
  });
}

GLuint Create2DTexture(const std::string& a_sFileName,
 GLint a_nMagFilterParam, GLint a_nMinFilterParm,
 GLint a_nTexHorizontalWrapParam, GLint a_nTexVerticalWrapParam,
 GLint a_nInternalFormat, GLint a_nImageFormat) {
  GLsizei wWidth, wHeight, wNbrChannels;
  unsigned char *wImage = nullptr;
  
  if (!LoadImage(a_sFileName, &wImage, wWidth, wHeight, wNbrChannels)) {
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

  FreeImage(wImage);
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
    GLsizei wWidth, wHeight, wNbrChannels;
    unsigned char *wImage = nullptr;

    if (!LoadImage(textureFileName, &wImage, wWidth, wHeight, wNbrChannels)) {
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

    FreeImage(wImage);
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