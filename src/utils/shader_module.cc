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
#include "utils/shader_module.hh"

#include <algorithm>
#include <vector>
#include <map>
#include <fstream>
#include <mutex>

#include "utils/shader_factory.hh"
#include "utils/imgui/imgui_log.h"

namespace shader {
namespace module {
namespace {
using SSBOCapacity = GLuint;
using BindingPoint = GLuint;
using ID = GLuint;
ID                                                    current_program;
std::map<GLuint, std::map<std::string, GLuint> >      attrib_list;
std::map<BindingPoint, ID >                           uniform_block_list;
std::map<BindingPoint, std::pair<ID, SSBOCapacity> >  SSBO_list;

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
    ImGuiLog::GetInstance().AddLog("shader_module::Terminate -> Deleting UBOs.\n");
    for (auto uniformBlocks : uniform_block_list) {
      glDeleteBuffers(1, &uniformBlocks.second);
    }
    for (auto SSBO : SSBO_list) {
      glDeleteBuffers(1, &SSBO.second.first);
    }
  });
}

GLuint GetProgramID() { 
  return current_program; 
}

GLuint GetAttribLocation(const std::string& a_rAttrib, GLuint a_unProgramID) { 
  return attrib_list[a_unProgramID][a_rAttrib]; 
}

GLint GetUniformLocation(GLuint a_unProgramID, const std::string& a_rUniform) {
  GLint wReturnValue = glGetUniformLocation(a_unProgramID, a_rUniform.c_str());
  if (wReturnValue == -1) {
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_module::GetUniformLocation -> Bad uniform name.\n");
  }
  return wReturnValue;
}

void LoadFromPreCompiledText(GLenum type, const std::string& src) {
  //TODO:
}
void LoadFromPreCompiledFile(GLenum type, const char* fileName) {
  //TODO:
}

void RegisterAttribute(std::string a_sAttrib, GLuint a_unProgramID) {
  if (attrib_list.count(a_unProgramID) == 0) {
    attrib_list[a_unProgramID] = {};
  }
  if (attrib_list.at(a_unProgramID).count(a_sAttrib) == 0) {
    attrib_list[a_unProgramID][a_sAttrib] =
      glGetAttribLocation(a_unProgramID, a_sAttrib.c_str());
  }
  else {
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_module::RegisterAttribute -> Trying to register uniform that already\n");
    ImGuiLog::GetInstance().AddLog("exists for program %d. Will ignore instruction...\n", a_unProgramID);
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
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_module::RegisterGlobalUniform -> Trying to register binding point\n");
    ImGuiLog::GetInstance().AddLog("%d which is already registered. Will ignore instruction...\n", a_unBindingPoint);
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
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_module::RegisterGlobalUniform -> Trying to register binding point\n");
    ImGuiLog::GetInstance().AddLog("%d which is already registered. Will ignore instruction...\n", a_unBindingPoint);
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
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_module::SetUniformBlockValue -> Trying to set value to binding point\n");
    ImGuiLog::GetInstance().AddLog("%d without registering an UBO. Will ignore instruction...\n", a_unBindingPoint);
  }
}

void RegisterSSBOBlock(GLuint a_unBindingPoint, 
  GLuint a_unSSBOSize, void* a_pData, GLenum a_eUsage) {
  if (SSBO_list.count(a_unBindingPoint) == 0) {
    GLuint unSSBOID;
    glGenBuffers(1, &unSSBOID);
    SSBO_list[a_unBindingPoint] = std::make_pair(unSSBOID, a_unSSBOSize);
    ImGuiLog::GetInstance().AddLog("shader_module::RegisterSSBOBlock -> Generated a SSBO, ID = %d\n", unSSBOID);
    UpdateSSBOBlockData(a_unBindingPoint, a_unSSBOSize, a_pData, a_eUsage);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, a_unBindingPoint, unSSBOID);
    SSBO_list[a_unBindingPoint] = std::make_pair(unSSBOID, a_unSSBOSize);
  }
  else {
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_module::RegisterSSBO -> Trying to register binding point \n");
    ImGuiLog::GetInstance().AddLog("%d which is already registered.Will ignore instruction...\n", a_unBindingPoint);
  }
}

void UpdateSSBOBlockData(GLuint a_unBindingPoint,
  GLuint a_unSSBOSize, void* a_pData, GLenum a_eUsage) {
  auto SSBO = SSBO_list.find(a_unBindingPoint);
  if (SSBO != SSBO_list.end()) {
    GLuint unSSBOID = SSBO->second.first;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, unSSBOID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, a_unSSBOSize, a_pData, a_eUsage);
    ImGuiLog::GetInstance().AddLog("shader_module::UpdateSSBOBlockData -> Updated SSBO with ID %d, final size = %d\n", unSSBOID, a_unSSBOSize);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); //unbind
  }
  else {
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_module::UpdateSSBOBlockData -> Trying to update value to binding point \n");
    ImGuiLog::GetInstance().AddLog("%d without registering an SSBO. Will ignore instruction...\n", a_unBindingPoint);
  }
}

void SetSSBOBlockSubData(GLuint a_unBindingPoint,
  GLuint a_unOffset, void* a_pData, GLuint a_unSize) {
  auto SSBO = SSBO_list.find(a_unBindingPoint);
  if (SSBO != SSBO_list.end()) {
    GLuint unSSBOID = SSBO->second.first;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, unSSBOID);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, a_unOffset, a_unSize, a_pData);
    ImGuiLog::GetInstance().AddLog("shader_module::SetSSBOBlockSubData -> Set sub data in SSBO with ID %d, offset = %d, size = %d\n", unSSBOID, a_unOffset, a_unSize);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  }
  else {
    ImGuiLog::GetInstance().AddLog("[ERROR]shader_module::SetSSBOValue -> Trying to set value to binding point \n");
    ImGuiLog::GetInstance().AddLog("%d without registering an SSBO. Will ignore instruction...\n", a_unBindingPoint);
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