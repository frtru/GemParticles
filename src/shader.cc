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

#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <mutex>

namespace gem {
namespace particle {
namespace shader_manager {
namespace {
// For each program ID, we have a list of
// compiled/link shader sources
GLuint                                              current_program;
std::map<GLuint, std::vector< GLuint > >            shader_programs;     
std::map<GLuint, std::string>                       compiled_shaders;
std::vector<GLuint>                                 compilation_cache;

std::map<GLuint, std::map<std::string, GLuint> >    attrib_list;
std::map<GLuint, std::map<std::string, GLuint> >    uniform_location_list;
std::map<GLuint, GLuint >                           uniform_block_list;

std::once_flag                    init_flag;
std::once_flag                    terminate_flag;
}

void Init() {
  std::call_once(init_flag, [&]() {
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
    std::cout << "shader_manager::Terminate -> Deleting shader programs." << std::endl;
    for (auto shader : shader_programs) {
      glDeleteProgram(shader.first);
    }
    std::cout << "shader_manager::Terminate -> Deleting UBOs." << std::endl;
    for (auto uniformBlocks : uniform_block_list) {
      glDeleteBuffers(1, &uniformBlocks.second);
    }
  });
}

bool AddShader(std::string a_sFileName, GLenum a_eShaderType) {
  for (auto shader : compiled_shaders) {
    // If shader file was already compiled
    if (shader.second == a_sFileName) {
      // If the compiled shader ID is not already
      // in compilation cache
      if (std::count(compilation_cache.begin(),
        compilation_cache.end(), shader.first) == 0) {
        compilation_cache.push_back(shader.first);
        return true;
      }
      else {
        std::cerr << "shader_manager::AddShaderToCompile -> Trying to add multiple times " << std::endl
          << "compiled shader " << a_sFileName << " into compilation cache. Will ignore instruction..." << std::endl;
        return false;
      }
    }
  }
  // First encounter of file a_sFileName, compile it
  return CompileShaderFile(a_sFileName, a_eShaderType);
}

bool CompileShaderFile(std::string a_sFileName, GLenum a_eShaderType) {
  std::ifstream fparser;
  fparser.open(a_sFileName, std::ios_base::in);
  if (fparser) {
    ///read + load
    std::string buffer(
      std::istreambuf_iterator<char>(fparser), 
      (std::istreambuf_iterator<char>())
    );
    return CompileShaderText(buffer, a_eShaderType, a_sFileName);
  }
  std::cerr << "shader_manager::CompileShaderFile -> "
    << "Invalid fileName path : "
    << a_sFileName << std::endl;
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
  if (status == GL_FALSE) {
    GLint infoLogSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize);
    GLchar *infoLog = new GLchar[infoLogSize];
    glGetShaderInfoLog(shader, infoLogSize, nullptr, infoLog);
    std::cerr << "shader_manager::CompileShaderText -> "
      << infoLog << std::endl;
    delete[] infoLog;
    return false;
  }
  compilation_cache.push_back(shader);
  compiled_shaders[shader] = a_sFileName;
  return true;
}

GLuint CreateProgram() {
  // Check if compilation_cache is not empty before processing
  if (!compilation_cache.size()) {
    std::cerr << "shader_manager::CreateProgram -> Trying to create a program from an empty " << std::endl
      << "compilation cache. Returning 0xFFFFFFFF..." << std::endl;
    return 0xFFFFFFFF;
  }
  // By sorting the compilation cache, we
  // make sure that every list in shader_programs
  // is already sorted.
  std::sort(compilation_cache.begin(), compilation_cache.end());
  for (auto shader : shader_programs) {
    // Before creating a new program, we check if cache's
    // content is already in the shader_programs list
    if (shader.second == compilation_cache) {
      compilation_cache.clear();
      return shader.first;
    }
  }
  // At this point, we are facing a new
  // program cache, create a new program
  GLuint programID = glCreateProgram();
  for (GLuint shaderID : compilation_cache) {
    glAttachShader(programID, shaderID);
  }

  ///link + check
  GLint status;
  glLinkProgram(programID);
  glGetProgramiv(programID, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogSize;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogSize);
    GLchar *infoLog = new GLchar[infoLogSize];
    glGetProgramInfoLog(programID, infoLogSize, nullptr, infoLog);
    std::cerr << "shader_manager::CreateProgram() -> "
      << infoLog << std::endl;
    delete[] infoLog;
    return 0xFFFFFFFF;
  }
  // Detach + delete shaders
  for (GLuint shaderID : compilation_cache) {
    glDetachShader(programID, shaderID);
    glDeleteShader(shaderID);
  }

  shader_programs[programID] = compilation_cache;
  compilation_cache.clear();
  return programID;
}

GLuint GetProgramID() { 
  return current_program; 
}

GLuint GetAttribLocation(const std::string& a_rAttrib, GLuint a_unProgramID) { 
  return attrib_list[a_unProgramID][a_rAttrib]; 
}

GLuint GetUniformLocation(const std::string& a_rUniform, GLuint a_unProgramID) { 
  return uniform_location_list[a_unProgramID][a_rUniform]; 
}

void LoadFromPreCompiledText(GLenum type, const std::string& src) {
  //TODO:
}
void LoadFromPreCompiledFile(GLenum type, const char* fileName) {
  //TODO:
}

void RegisterAttribute(std::string a_sAttrib, GLuint a_unProgramID) {
  if (attrib_list.at(a_unProgramID).count(a_sAttrib) == 0) {
    attrib_list[a_unProgramID][a_sAttrib] =
      glGetAttribLocation(a_unProgramID, a_sAttrib.c_str());
  }
  else {
    std::cerr << "shader_manager::RegisterUniform -> Trying to register uniform that already" << std::endl
      << "exists for program " << a_unProgramID << ". Will ignore instruction..." << std::endl;
  }
}

void RegisterUniform(std::string a_sUniform, GLuint a_unProgramID) {
  if (uniform_location_list.at(a_unProgramID).count(a_sUniform) == 0) {
    uniform_location_list[a_unProgramID][a_sUniform] =
      glGetUniformLocation(a_unProgramID, a_sUniform.c_str());
  }
  else {
    std::cerr << "shader_manager::RegisterUniform -> Trying to register uniform that already" << std::endl
      << "exists for program " << a_unProgramID << ". Will ignore instruction..." << std::endl;
  }
}

// Before OpenGL 4.2 compatible version
void RegisterGlobalUniformBlock(GLuint a_unBindingPoint, 
  GLuint a_unUBOSize, 
  GLuint a_unProgramID, 
  std::string a_sUniformBlock) {
  if (uniform_block_list.count(a_unBindingPoint) == 0) {

    GLuint unUniformBlockIndex = glGetUniformBlockIndex(a_unProgramID, a_sUniformBlock.c_str());
    glUniformBlockBinding(a_unProgramID, unUniformBlockIndex, a_unBindingPoint);

    GLuint unUBOID;
    glGenBuffers(1, &unUBOID);
    glBindBuffer(GL_UNIFORM_BUFFER, unUBOID);
    glBufferData(GL_UNIFORM_BUFFER, a_unUBOSize, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER,
      a_unBindingPoint,
      unUBOID);
    uniform_block_list[a_unBindingPoint] = unUBOID;
  }
  else {
    std::cerr << "shader_manager::RegisterGlobalUniform -> Trying to register binding point " << a_unBindingPoint << std::endl
      << "which is already registered. Will ignore instruction..." << std::endl;
  }
}

// OpenGL 4.2 lets us avoid calling 
// glGetUniformBlockIndex and glUniformBlockBinding.
void RegisterGlobalUniformBlock(GLuint a_unBindingPoint, 
  GLuint a_unUBOSize) {
  if (uniform_block_list.count(a_unBindingPoint) == 0) {
    GLuint unUBOID;
    glGenBuffers(1, &unUBOID);
    glBindBuffer(GL_UNIFORM_BUFFER, unUBOID);
    glBufferData(GL_UNIFORM_BUFFER, a_unUBOSize, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER,
      a_unBindingPoint,
      unUBOID);
    uniform_block_list[a_unBindingPoint] = unUBOID;
  }
  else {
    std::cerr << "shader_manager::RegisterGlobalUniform -> Trying to register binding point " << a_unBindingPoint << std::endl
      << "which is already registered. Will ignore instruction..." << std::endl;
  }
}

void SetUniformBlockValue(GLuint a_unBindingPoint, 
  GLuint a_unOffset, 
  GLuint a_unUBOSize, 
  void* a_pValue) {
  auto uniform_block = uniform_block_list.find(a_unBindingPoint);
  if (uniform_block != uniform_block_list.end()) {
    GLuint unUBOID = uniform_block->second;
    glBindBuffer(GL_UNIFORM_BUFFER, unUBOID);
    glBufferSubData(GL_UNIFORM_BUFFER, 
      a_unOffset, 
      a_unUBOSize,
      a_pValue);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
  else {
    std::cerr << "shader_manager::SetUniformBlockValue -> Trying to set value to binding point " << a_unBindingPoint << std::endl
      << "without registering an UBO. Will ignore instruction..." << std::endl;
  }
}

void Use(GLuint a_unProgramID)   { 
  if (a_unProgramID != current_program) {
    current_program = a_unProgramID;
    glUseProgram(current_program);
  }
}

void Detach() { 
  current_program = 0;
  glUseProgram(0); 
}
} /* namespace shader_manager */
} /* namespace particle */
} /* namespace gem */