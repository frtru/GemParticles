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
#ifndef SHADER_MODULE_HH
#define SHADER_MODULE_HH

#include <string>
#include <GL/glew.h>

namespace shader {
namespace module {
void Init();
void Terminate();

GLuint GetAttribLocation(const std::string& a_rAttrib, GLuint a_unProgramID);
GLint GetUniformLocation(const std::string& a_rUniform, GLuint a_unProgramID);

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

void Use(GLuint a_unProgramID);
void Detach();
} /* namespace module */
} /* namespace shader */
#endif /* end of include guard: SHADER_MODULE_HH */
