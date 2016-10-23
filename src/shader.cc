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
#include "shader.hh"

#include <iostream>
#include <fstream>

namespace Gem {
namespace Particle {
ShaderManager::ShaderManager()
  :m_unProgram(0), m_unNumShaders(0) {
  m_unShaders[VERTEX_SHADER] = 0;
  m_unShaders[FRAGMENT_SHADER] = 0;
  m_unShaders[GEOMETRY_SHADER] = 0;
  m_unShaders[PIXEL_SHADER] = 0;
  m_attribList.clear();
  m_unifLocationList.clear();
}

ShaderManager::~ShaderManager() {
  if (m_unProgram != -1) {
    Dispose();
  }
}

void ShaderManager::LoadFromFile(GLenum which, const char* fileName) {
  std::ifstream fparser;
  fparser.open(fileName, std::ios_base::in);
  if (fparser) {
    ///read + load
    std::string buffer(std::istreambuf_iterator<char>(fparser), (std::istreambuf_iterator<char>()));
    LoadFromText(which, buffer);
  }
  else {
    std::cerr << "ShaderManager::LoadFromFile -> "
      << "Invalid fileName path : "
      << fileName << std::endl;
  }
}

void ShaderManager::LoadFromText(GLenum type, const std::string& text) {
  GLuint shader = glCreateShader(type);
  const char* cstr = text.c_str();
  glShaderSource(shader, 1, &cstr, nullptr);

  ///compile + check shader load status
  GLint status;
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize);
    GLchar *infoLog = new GLchar[infoLogSize];
    glGetShaderInfoLog(shader, infoLogSize, nullptr, infoLog);
    std::cerr << "ShaderManager::LoadFromText -> " 
              << infoLog << std::endl;
    delete[] infoLog;
  }
  m_unShaders[m_unNumShaders++] = shader;
}

void ShaderManager::CreateAndLink() {
  m_unProgram = glCreateProgram();
  if (m_unShaders[VERTEX_SHADER] != 0)
    glAttachShader(m_unProgram, m_unShaders[VERTEX_SHADER]);
  if (m_unShaders[FRAGMENT_SHADER] != 0)
    glAttachShader(m_unProgram, m_unShaders[FRAGMENT_SHADER]);
  if (m_unShaders[GEOMETRY_SHADER] != 0)
    glAttachShader(m_unProgram, m_unShaders[GEOMETRY_SHADER]);
  if (m_unShaders[PIXEL_SHADER] != 0)
    glAttachShader(m_unProgram, m_unShaders[PIXEL_SHADER]);

  ///link + check
  GLint status;
  glLinkProgram(m_unProgram);
  glGetProgramiv(m_unProgram, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogSize;
    glGetProgramiv(m_unProgram, GL_INFO_LOG_LENGTH, &infoLogSize);
    GLchar *infoLog = new GLchar[infoLogSize];
    glGetProgramInfoLog(m_unProgram, infoLogSize, nullptr, infoLog);
    delete[] infoLog;
  }

  glDetachShader(m_unProgram, m_unShaders[VERTEX_SHADER]);
  glDetachShader(m_unProgram, m_unShaders[FRAGMENT_SHADER]);
  glDetachShader(m_unProgram, m_unShaders[GEOMETRY_SHADER]);
  glDetachShader(m_unProgram, m_unShaders[PIXEL_SHADER]);

  glDeleteShader(m_unShaders[VERTEX_SHADER]);
  glDeleteShader(m_unShaders[FRAGMENT_SHADER]);
  glDeleteShader(m_unShaders[GEOMETRY_SHADER]);
  glDeleteShader(m_unShaders[PIXEL_SHADER]);
}

void ShaderManager::Bind() const {
  glUseProgram(m_unProgram);
}

void ShaderManager::Detach() const {
  glUseProgram(0);
}

void ShaderManager::RegisterAttribute(const char* attrib) {
  m_attribList[attrib] = glGetAttribLocation(m_unProgram, attrib);
}

void ShaderManager::RegisterUniform(const char* unif) {
  m_unifLocationList[unif] = glGetUniformLocation(m_unProgram, unif);
}

void ShaderManager::Dispose() {
  glDeleteProgram(m_unProgram);
  m_unProgram = -1;
}
} /* namespace Particle */
} /* namespace Gem */