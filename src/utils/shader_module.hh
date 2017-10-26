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
#include "glm/glm.hpp"

namespace shader {
namespace module {
void Init();
void Terminate();

GLuint GetAttribLocation(const std::string& a_rAttrib, GLuint a_unProgramID);
void RegisterAttribute(std::string a_sAttrib, GLuint a_unProgramID); 

GLint GetUniformLocation(GLuint a_unProgramID, const std::string& a_rUniform);
inline void SetUniformBool(GLuint a_unProgramID, const std::string &a_sName, bool a_Value)
{
  glUniform1i(GetUniformLocation(a_unProgramID, a_sName.c_str()), (int)a_Value);
}
inline void SetUniformInt(GLuint a_unProgramID, const std::string &a_sName, int a_Value)
{
  glUniform1i(GetUniformLocation(a_unProgramID, a_sName.c_str()), a_Value);
}
inline void SetUniformFloat(GLuint a_unProgramID, const std::string &a_sName, float a_Value)
{
  glUniform1f(GetUniformLocation(a_unProgramID, a_sName.c_str()), a_Value);
}
inline void SetUniformVec2(GLuint a_unProgramID, const std::string &a_sName, const glm::vec2 &a_Value)
{
  glUniform2fv(GetUniformLocation(a_unProgramID, a_sName.c_str()), 1, &a_Value[0]);
}
inline void SetUniformVec2(GLuint a_unProgramID, const std::string &a_sName, float a_X, float a_Y)
{
  glUniform2f(GetUniformLocation(a_unProgramID, a_sName.c_str()), a_X, a_Y);
}
inline void SetUniformVec3(GLuint a_unProgramID, const std::string &a_sName, const glm::vec3 &a_Value)
{
  glUniform3fv(GetUniformLocation(a_unProgramID, a_sName.c_str()), 1, &a_Value[0]);
}
inline void SetUniformVec3(GLuint a_unProgramID, const std::string &a_sName, float a_X, float a_Y, float a_Z)
{
  glUniform3f(GetUniformLocation(a_unProgramID, a_sName.c_str()), a_X, a_Y, a_Z);
}
inline void SetUniformVec4(GLuint a_unProgramID, const std::string &a_sName, const glm::vec4 &a_Value)
{
  glUniform4fv(GetUniformLocation(a_unProgramID, a_sName.c_str()), 1, &a_Value[0]);
}
inline void SetUniformVec4(GLuint a_unProgramID, const std::string &a_sName, float a_X, float a_Y, float a_Z, float a_W)
{
  glUniform4f(GetUniformLocation(a_unProgramID, a_sName.c_str()), a_X, a_Y, a_Z, a_W);
}
inline void SetUniformMat2(GLuint a_unProgramID, const std::string &a_sName, const glm::mat2 &a_Mat)
{
  glUniformMatrix2fv(GetUniformLocation(a_unProgramID, a_sName.c_str()), 1, GL_FALSE, &a_Mat[0][0]);
}
inline void SetUniformMat3(GLuint a_unProgramID, const std::string &a_sName, const glm::mat3 &a_Mat)
{
  glUniformMatrix3fv(GetUniformLocation(a_unProgramID, a_sName.c_str()), 1, GL_FALSE, &a_Mat[0][0]);
}
inline void SetUniformMat4(GLuint a_unProgramID, const std::string &a_sName, const glm::mat4 &a_Mat)
{
  glUniformMatrix4fv(GetUniformLocation(a_unProgramID, a_sName.c_str()), 1, GL_FALSE, &a_Mat[0][0]);
}

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

// Shader Storage Buffer Object can be used since OpenGL core profile 4.3
void RegisterSSBOBlock(GLuint a_unBindingPoint, GLuint a_unSSBOSize, void* a_pData, GLenum a_eUsage = GL_DYNAMIC_DRAW);
void UpdateSSBOBlockData(GLuint a_unBindingPoint, GLuint a_unSSBOSize, void* a_pData, GLenum a_eUsage = GL_DYNAMIC_DRAW);
void SetSSBOBlockSubData(GLuint a_unBindingPoint, GLuint a_unOffset, void* a_pData, GLuint a_unSize);

void Use(GLuint a_unProgramID);
void Detach();
} /* namespace module */
} /* namespace shader */
#endif /* end of include guard: SHADER_MODULE_HH */
