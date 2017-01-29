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
  /* Poll for and process events */
  glfwPollEvents();

  /* Swap front and back buffers */
  glfwSwapBuffers(m_pWindow);

  /* Once the buffers are swapped, lets clear the canvas*/
  /* TODO: NOTE: This should be placed as the first thing done
   in the rendering loop (which currently is the same thing as being
   placed here, but its a better practice to do the first one*/
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool OpenGLContext::PollWindowClosedEvent() {
  return glfwWindowShouldClose(m_pWindow) != 0 ||
         glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

void OpenGLContext::Reshape(int a_width, int a_height) {

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

  /* Ensure we can capture keys being pressed */
  glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
  
  //TODO: Insert other glfw parameters here

  // GLEW initialization
  if (GLEW_OK != glewInit()) {
    std::cerr << "GLEW is not initialized!" << std::endl;
  }

  // OpenGL initialization
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, 640, 480);
}

void OpenGLContext::TerminateImpl() {
  glfwTerminate();
}
} /* namespace Particle */
} /* namespace Gem */
