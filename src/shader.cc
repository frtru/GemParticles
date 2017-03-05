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
#include <mutex>

namespace gem {
namespace particle {
namespace shader_manager {
namespace {
GLuint                        program;
std::map<GLenum, GLuint>      shaders;
std::map<std::string, GLuint> attrib_list;
std::map<std::string, GLuint> uniform_location_list;

std::once_flag                init_flag;
std::once_flag                terminate_flag;
}

void Init() {
  std::call_once(init_flag, [&]() {
    program = 0;
    shaders = {
      { GL_VERTEX_SHADER, 0 },
      { GL_FRAGMENT_SHADER, 0 },
      { GL_GEOMETRY_SHADER, 0 },
      { GL_TESS_CONTROL_SHADER, 0 },
      { GL_TESS_EVALUATION_SHADER, 0 },
      { GL_COMPUTE_SHADER, 0 } };
    attrib_list.clear();
    uniform_location_list.clear();
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
    if (program != -1) {
      Dispose();
    }
  });
}

GLuint GetProgramID()                         { return program; }
GLuint GetAttribLocation(const char* attrib)  { return attrib_list[attrib]; }
GLuint GetUniformLocation(const char* unif)   { return uniform_location_list[unif]; }

void LoadFromFile(GLenum which, const char* fileName) {
  std::ifstream fparser;
  fparser.open(fileName, std::ios_base::in);
  if (fparser) {
    ///read + load
    std::string buffer(std::istreambuf_iterator<char>(fparser), (std::istreambuf_iterator<char>()));
    LoadFromText(which, buffer);
  }
  else {
    std::cerr << "shader_manager::LoadFromFile -> "
      << "Invalid fileName path : "
      << fileName << std::endl;
  }
}

void LoadFromText(GLenum type, const std::string& text) {
  //Type must be one of the supported and registered
  //types in the initialization
  if (shaders.count(type) == 0) {
    std::cerr << "shader_manager::LoadFromText -> "
              << "Type received is not recognized/supported" << std::endl;
  }

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
    std::cerr << "shader_manager::LoadFromText -> " 
              << infoLog << std::endl;
    delete[] infoLog;
    return;
  }
  shaders[type] = shader;
}

void LoadFromPreCompiledText(GLenum type, const std::string& src) {
  //TODO:
}
void LoadFromPreCompiledFile(GLenum type, const char* fileName) {
  //TODO:
}

void CreateAndLink() {
  program = glCreateProgram();
  if (shaders[GL_VERTEX_SHADER] != 0)
    glAttachShader(program, shaders[GL_VERTEX_SHADER]);
  if (shaders[GL_FRAGMENT_SHADER] != 0)
    glAttachShader(program, shaders[GL_FRAGMENT_SHADER]);
  if (shaders[GL_GEOMETRY_SHADER] != 0)
    glAttachShader(program, shaders[GL_GEOMETRY_SHADER]);
  if (shaders[GL_TESS_CONTROL_SHADER] != 0)
    glAttachShader(program, shaders[GL_TESS_CONTROL_SHADER]);
  if (shaders[GL_TESS_EVALUATION_SHADER] != 0)
    glAttachShader(program, shaders[GL_TESS_EVALUATION_SHADER]);
  if (shaders[GL_COMPUTE_SHADER] != 0)
    glAttachShader(program, shaders[GL_COMPUTE_SHADER]);

  ///link + check
  GLint status;
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogSize);
    GLchar *infoLog = new GLchar[infoLogSize];
    glGetProgramInfoLog(program, infoLogSize, nullptr, infoLog);
    delete[] infoLog;
  }

  // TODO: Shouldn't this be in the dispose/detach function
  glDetachShader(program, shaders[GL_VERTEX_SHADER]);
  glDetachShader(program, shaders[GL_FRAGMENT_SHADER]);
  glDetachShader(program, shaders[GL_GEOMETRY_SHADER]);
  glDetachShader(program, shaders[GL_TESS_CONTROL_SHADER]);
  glDetachShader(program, shaders[GL_TESS_EVALUATION_SHADER]);
  glDetachShader(program, shaders[GL_COMPUTE_SHADER]);

  glDeleteShader(shaders[GL_VERTEX_SHADER]);
  glDeleteShader(shaders[GL_FRAGMENT_SHADER]);
  glDeleteShader(shaders[GL_GEOMETRY_SHADER]);
  glDeleteShader(shaders[GL_TESS_CONTROL_SHADER]);
  glDeleteShader(shaders[GL_TESS_EVALUATION_SHADER]);
  glDeleteShader(shaders[GL_COMPUTE_SHADER]);
}

void RegisterAttribute(const char* attrib) {
  attrib_list[attrib] = glGetAttribLocation(program, attrib);
}

void RegisterUniform(const char* unif) {
  uniform_location_list[unif] = glGetUniformLocation(program, unif);
}

void Bind()   { glUseProgram(program); }
void Detach() { glUseProgram(0); }

void Dispose() {
  glDeleteProgram(program);
  program = -1;
}
} /* namespace shader_manager */
} /* namespace particle */
} /* namespace gem */