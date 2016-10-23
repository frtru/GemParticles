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
//TODO : Transform this into a namespace just like the app namespace
class ShaderManager {
public:
  // Standard enums are
  //GL_COMPUTE_SHADER
  //GL_VERTEX_SHADER
  //GL_TESS_CONTROL_SHADER
  //GL_TESS_EVALUATION_SHADER
  //GL_GEOMETRY_SHADER
  //GL_FRAGMENT_SHADER

  // TODO: Why is this enum there?
  enum ShaderType {
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    GEOMETRY_SHADER,
    PIXEL_SHADER,
    // TODO: Uncomment the following when they will be supported in the .cc file
    // TESS_CONTROL_SHADER,
    // TESS_EVALUATION_SHADER,
    // COMPUTE_SHADER,
    COUNT
  };

public:
  ShaderManager();
  ~ShaderManager();

  GLuint GetProgramID() const { return m_unProgram; }
  GLuint GetAttribLocation(const char* attrib){ return m_attribList[attrib]; }
  GLuint GetUniformLocation(const char* unif){ return m_unifLocationList[unif]; }

  void LoadFromText(GLenum type, const std::string& src);
  void LoadFromFile(GLenum type, const char* fileName);
  void LoadFromPreCompiledText(GLenum type, const std::string& src) {}
  void LoadFromPreCompiledFile(GLenum type, const char* fileName) {}
  void CreateAndLink();
  void RegisterAttribute(const char* attrib);
  void RegisterUniform(const char* uniform);

  void Bind() const;
  void Detach() const;
  void Dispose();

private:

  unsigned int                  m_unNumShaders;
  GLuint                        m_unProgram;
  // TODO: change following for a map, with standard enums def as key
  // and GLuint as value for the return of glCreateShader
  GLuint                        m_unShaders[ShaderType::COUNT];  
  std::map<std::string, GLuint> m_attribList;
  std::map<std::string, GLuint> m_unifLocationList;
}; /* class ShaderManager*/
} /* namespace Particle */
} /* namespace Gem */
#endif /* end of include guard: SHADER_HH */
