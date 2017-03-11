/*************************************************************************
 * Copyright (c) 2016 Fran�ois Trudel
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
#ifndef SCENE_HH
#define SCENE_HH

// TODO: Should this be a singleton/namespace?
// Since there can be only one instance of a scene
// throughout the app...

// TODO: Add lights by registering their positions
// as an UBO in the shaders maybe?

#include <GL/glew.h>

namespace gem {
namespace particle {
class Scene{
private:
  static const GLfloat AXES_POINTS[];
public:
  Scene(bool a_isDebug = false);
  ~Scene();

  inline void SetDebugOption(bool a_isDebug) {
    m_bIsDebug = a_isDebug;
  }

  void Render();
private:
  void DrawAxes();

  bool    m_bIsDebug;
  GLuint  m_vertexArrayID;
  GLuint  m_vertexBufferID;
  GLuint  m_unProgramID;
}; /* class Scene*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: SCENE_HH */

