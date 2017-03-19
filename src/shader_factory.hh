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
#ifndef SHADER_FACTORY_HH
#define SHADER_FACTORY_HH

#include <GL/glew.h>
#include <string>

namespace shader {
namespace factory {
void Init();
void Terminate();

bool CompileShaderFile(std::string a_sFileName, GLenum a_eShaderType);
bool CompileShaderText(const std::string& a_rShaderText,
  GLenum a_eShaderType,
  std::string a_sFileName = "text");

void LoadFromPreCompiledText(GLenum type, const std::string& src);
void LoadFromPreCompiledFile(GLenum type, const char* fileName);

GLuint CreateProgram();
} /* namespace factory */
} /* namespace shader */
#endif /* end of include guard: SHADER_FACTORY_HH */
