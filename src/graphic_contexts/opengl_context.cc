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
#include "graphic_contexts/opengl_context.hh"

#include <iostream>

namespace gem {
namespace particle {
OpenGLContext::OpenGLContext() 
  : m_pWindow(nullptr) {}

OpenGLContext::~OpenGLContext() {
  // TODO: When closing window first instead of console
  // it tries to delete the GLFWwindow* which is already
  // deleted, maybe the shaded_ptr isnt a good idea
}

void* OpenGLContext::GetWindowHandle() const {
  return m_pWindow;
}

std::size_t OpenGLContext::GetWindowWidth() const {
  int width = 0;
  glfwGetWindowSize(m_pWindow, &width, NULL);
  return width;
}

std::size_t OpenGLContext::GetWindowHeight() const {
  int height = 0;
  glfwGetWindowSize(m_pWindow, NULL, &height);
  return height;
}

void OpenGLContext::Update() {
  /* Poll for and process events */
  glfwPollEvents();

  /* Swap front and back buffers */
  glfwSwapBuffers(m_pWindow);

  /* Once the buffers are swapped, lets clear the canvas*/
  /* NOTE: This should be placed as the first thing done
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
  m_pWindow = glfwCreateWindow(640, 480, "GemParticles", NULL, NULL);
  if (!m_pWindow) {
    glfwTerminate();
    std::cerr << "OpenGLSetup -> glfwCreateWindow failed!" << std::endl;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(m_pWindow);

  /* Ensure we can capture keys being pressed */
  glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
  //glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  
  //TODO: Insert other glfw parameters here

  // GLEW initialization
  if (GLEW_OK != glewInit()) {
    std::cerr << "GLEW is not initialized!" << std::endl;
  }

  // OpenGL initialization
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, 640, 480);

  //glEnable(GL_POINT_SMOOTH);
  glEnable(GL_CULL_FACE);
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  // TODO: Might have to send the size depending on the 
  // type of particles sent...
  //glPointSize(0.1f);
}

void OpenGLContext::TerminateImpl() {
  std::cout << "OpenGLContext::TerminateImpl -> Deleting glfw context." << std::endl;
  glfwTerminate();
}
} /* namespace particle */
} /* namespace gem */
