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
#ifndef SHADER_HH
#define SHADER_HH

#include <GL/glew.h>
#include <string>

namespace gem {
namespace particle {
namespace shader_manager {
void Init();
void Terminate();

bool AddShader(std::string a_sFileName, GLenum a_eShaderType);
bool CompileShaderFile(std::string a_sFileName, GLenum a_eShaderType);
bool CompileShaderText(const std::string& a_rShaderText, 
  GLenum a_eShaderType, 
  std::string a_sFileName = "text");
void LoadFromPreCompiledText(GLenum type, const std::string& src);
void LoadFromPreCompiledFile(GLenum type, const char* fileName);

GLuint GetAttribLocation(const std::string& a_rAttrib, GLuint a_unProgramID);
GLuint GetUniformLocation(const std::string& a_rUniform, GLuint a_unProgramID);

void RegisterAttribute(std::string a_sAttrib, GLuint a_unProgramID);
void RegisterUniform(std::string a_sUniform, GLuint a_unProgramID);
// Before OpenGL 4.2 compatible version
void RegisterGlobalUniformBlock(GLuint a_unBindingPoint, 
  GLuint a_unUBOSize, 
  GLuint a_unProgramID, 
  std::string a_sUniformBlock);
// OpenGL 4.2+ version
void RegisterGlobalUniformBlock(GLuint a_unBindingPoint, GLuint a_unUBOSize);

void SetUniformBlockValue(GLuint a_unBindingPoint,
  GLuint a_unOffset,
  GLuint a_unUBOSize,
  void* a_pValue);

GLuint CreateProgram();
void Use(GLuint a_unProgramID);
void Detach();
} /* namespace shader_manager*/
} /* namespace particle */
} /* namespace gem */
#endif /* end of include guard: SHADER_HH */
