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
#include "opengl_context.hh"

#include <iostream>

namespace Gem {
namespace Particle {
OpenGLContext::OpenGLContext() 
  : m_pWindow(nullptr) {}

OpenGLContext::~OpenGLContext() {

}

void OpenGLContext::Update() {
  /* Swap front and back buffers */
  glfwSwapBuffers(m_pWindow);

  /* Poll for and process events */
  glfwPollEvents();
}

int OpenGLContext::PollWindowClosedEvent() {
  return glfwWindowShouldClose(m_pWindow);
}

void OpenGLContext::InitImpl() {
  // Provide a way to set windowed mode or not, size and other useful parameters

  // GLFW initialization
  /* Initialize the library */
  if (!glfwInit())
    std::cerr << "OpenGLSetup -> glfwInit failed!" << std::endl;

  /* Create a windowed mode window and its OpenGL context */
  m_pWindow = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!m_pWindow) {
    glfwTerminate();
    std::cerr << "OpenGLSetup -> glfwCreateWindow failed!" << std::endl;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(m_pWindow);

  //Context/OpenGL initialization
  //TODO:

  // GLEW initialization
  if (GLEW_OK != glewInit()) {
    std::cerr << "GLEW is not initialized!" << std::endl;
  }
}

void OpenGLContext::TerminateImpl() {
  glfwTerminate();
}
} /* namespace Particle */
} /* namespace Gem */
