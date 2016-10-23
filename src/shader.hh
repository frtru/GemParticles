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
 *
 * Inspired from: http://codereview.stackexchange.com/questions/92924/modern-opengl-glsl-shader-class
*************************************************************************/
#ifndef SHADER_HH
#define SHADER_HH

#include <GL/glew.h>
#include <map>
#include <string>

namespace Gem {
namespace Particle {
namespace ShaderManager {
void Init();
void Terminate();

GLuint GetProgramID();
GLuint GetAttribLocation(const char* attrib);
GLuint GetUniformLocation(const char* unif);

void LoadFromText(GLenum type, const std::string& src);
void LoadFromFile(GLenum type, const char* fileName);
void LoadFromPreCompiledText(GLenum type, const std::string& src);
void LoadFromPreCompiledFile(GLenum type, const char* fileName);
void CreateAndLink();
void RegisterAttribute(const char* attrib);
void RegisterUniform(const char* uniform);

void Bind();
void Detach();
void Dispose();

} /* namespace ShaderManager*/
} /* namespace Particle */
} /* namespace Gem */
#endif /* end of include guard: SHADER_HH */
