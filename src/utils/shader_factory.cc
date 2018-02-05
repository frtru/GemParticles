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
#include "utils/shader_factory.hh"

#include <algorithm>
#include <vector>
#include <map>
#include <fstream>
#include <mutex>

#include "utils/imgui/imgui_log.h"

namespace shader {
namespace factory {
namespace {
// For each program ID, we have a list of
// compiled/link shader sources
std::map<GLuint, std::vector<std::string> >         shader_programs;
std::vector<GLuint>                                 compilation_cache;
std::vector<std::string>                            compilation_sources;
std::string                                         shader_base_path;
std::once_flag init_flag;
std::once_flag terminate_flag;
}
void Init() {
  std::call_once(init_flag, [&]() {
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
    ImGuiLog::GetInstance().AddLog("shader_factory::Terminate -> Deleting shader programs.\n");
    for (auto shader : shader_programs) {
      glDeleteProgram(shader.first);
    }
  });
}

void SetShadersFolderBasePath(const std::string & a_sPath) {
  shader_base_path = a_sPath;
}

bool CompileShaderFile(const std::string& a_sFileName, GLenum a_eShaderType) {
  std::ifstream fparser;
  fparser.open(shader_base_path + a_sFileName, std::ios_base::in);
  if (fparser) {
    ///read + load
    std::string buffer(
      std::istreambuf_iterator<char>(fparser), 
      (std::istreambuf_iterator<char>())
    );
    return CompileShaderText(buffer, a_eShaderType, shader_base_path + a_sFileName);
  }
  ImGuiLog::GetInstance().AddLog("[ERROR]shader_factory::CompileShaderFile -> Invalid fileName path : %s.\n", (shader_base_path + a_sFileName).c_str());
  return false;
}

bool CompileShaderText(const std::string& a_rShaderText, 
  GLenum a_eShaderType, 
  std::string a_sFileName) {
  GLuint shader = glCreateShader(a_eShaderType);
  const char* cstr = a_rShaderText.c_str();
  glShaderSource(shader, 1, &cstr, nullptr);

  ///compile + check shader load status
  GLint status;
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    GLint infoLogSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize);
    GLchar *infoLog = new GLchar[infoLogSize];
    glGetShaderInfoLog(shader, infoLogSize, nullptr, infoLog);
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_factory::CompileShaderText -> %s.\n", infoLog);
    delete[] infoLog;
    return false;
  }
  compilation_sources.push_back(a_sFileName);
  compilation_cache.push_back(shader);
  return true;
}

GLuint CreateProgram() {
  // Check if compilation_cache is not empty before processing
  if (compilation_cache.size() == 0) {
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_factory::CreateProgram -> Trying to create a program from an empty \n");
    ImGuiLog::GetInstance().AddLog("compilation cache. Returning 0xFFFFFFFF...\n");
    return 0xFFFFFFFF;
  }

  // Check if compilation_cache and compilation sources
  // are in sync
  if (compilation_cache.size() != compilation_sources.size()) {
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_factory::CreateProgram -> Incompatible sources<->cache sizes. Returning 0xFFFFFFFF...\n");
    return 0xFFFFFFFF;
  }

  // Check if all compilation_sources are 
  // already somewhere in a program in shader_programs
  std::sort(compilation_sources.begin(), compilation_sources.end());
  for (auto program : shader_programs) {
    // If sources that are about to be bound to a program
    // already have one, clear cache/sources and return it
    if (program.second == compilation_sources) {
      compilation_cache.clear();
      compilation_sources.clear();
      return program.first;
    }
  }

  GLuint programID = glCreateProgram();
  for (GLuint shaderID : compilation_cache) {
    glAttachShader(programID, shaderID);
  }

  ///link + check
  GLint status;
  glLinkProgram(programID);
  glGetProgramiv(programID, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    GLint infoLogSize;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogSize);
    GLchar *infoLog = new GLchar[infoLogSize];
    glGetProgramInfoLog(programID, infoLogSize, nullptr, infoLog);
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_factory::CreateProgram() -> %s.\n Returning 0xFFFFFFFF", infoLog);
    delete[] infoLog;
    return 0xFFFFFFFF;
  }
  // Detach + delete shaders
  for (GLuint shaderID : compilation_cache) {
    glDetachShader(programID, shaderID);
    glDeleteShader(shaderID);
  }

  shader_programs[programID] = compilation_sources;
  compilation_sources.clear();
  compilation_cache.clear();
  return programID;
}

void LoadFromPreCompiledText(GLenum type, const std::string& src) {
  //TODO:
}
void LoadFromPreCompiledFile(GLenum type, const char* fileName) {
  //TODO:
}
} /* namespace factory */
} /* namespace shader */