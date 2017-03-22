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
#include "shader_module.hh"

#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <mutex>

#include "shader_factory.hh"

namespace shader {
namespace module {
namespace {
GLuint                                              current_program;
std::map<GLuint, std::map<std::string, GLuint> >    attrib_list;
std::map<GLuint, std::map<std::string, GLuint> >    uniform_location_list;
std::map<GLuint, GLuint >                           uniform_block_list;

std::once_flag init_flag;
std::once_flag terminate_flag;
}

void Init() {
  std::call_once(init_flag, [&]() {
    shader::factory::Init();
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
    shader::factory::Terminate();
    std::cout << "shader_module::Terminate -> Deleting UBOs." << std::endl;
    for (auto uniformBlocks : uniform_block_list) {
      glDeleteBuffers(1, &uniformBlocks.second);
    }
  });
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
    std::cerr << "shader_module::RegisterUniform -> Trying to register uniform that already" << std::endl
      << "exists for program " << a_unProgramID << ". Will ignore instruction..." << std::endl;
  }
}

void RegisterUniform(std::string a_sUniform, GLuint a_unProgramID) {
  if (uniform_location_list.at(a_unProgramID).count(a_sUniform) == 0) {
    uniform_location_list[a_unProgramID][a_sUniform] =
      glGetUniformLocation(a_unProgramID, a_sUniform.c_str());
  }
  else {
    std::cerr << "shader_module::RegisterUniform -> Trying to register uniform that already" << std::endl
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
    std::cerr << "shader_module::RegisterGlobalUniform -> Trying to register binding point " << a_unBindingPoint << std::endl
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
    std::cerr << "shader_module::RegisterGlobalUniform -> Trying to register binding point " << a_unBindingPoint << std::endl
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
    std::cerr << "shader_module::SetUniformBlockValue -> Trying to set value to binding point " << a_unBindingPoint << std::endl
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
} /* namespace module */
} /* namespace shader */