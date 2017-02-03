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
#include "event_handler.hh"

#include <iostream>

#include <gl/glew.h>
#include <gl/glfw3.h>

#include "graphic_context.hh"
#include "particle_system.hh"
#include "camera.hh"
#include "shader.hh"

namespace Gem {
namespace Particle {
namespace EventHandler {
namespace {
enum MouseState {
  CAMERA_MOVING,
  FREE_CURSOR
}; 
MouseState                      mouse_state;
std::shared_ptr<GraphicContext> context_handle;

void MouseButtonCallBack(GLFWwindow* a_pWindow, int a_nButtonID, int a_nAction, int a_nMods) {
  // TODO: Handle all necessary cases
  switch (a_nButtonID) {
    case GLFW_MOUSE_BUTTON_LEFT:
      break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      if (a_nAction == GLFW_PRESS) {
        mouse_state = CAMERA_MOVING;
        std::cout << "input_state = camera moving" << std::endl;
      }
      else {
        std::cout << "input_state = free_cursor" << std::endl;
        mouse_state = FREE_CURSOR;
      }
      break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      break;
    default:
      break;
  }
}

void MouseCursorPositionCallback(GLFWwindow* a_pWindow, double a_dXPos, double a_dYPos) {
  if (mouse_state == CAMERA_MOVING) {

  }
}
}

void Init(const std::shared_ptr<GraphicContext>& a_pCtxt) {
  context_handle = a_pCtxt;
  mouse_state = FREE_CURSOR;
  GLFWwindow* window = static_cast<GLFWwindow*>(context_handle->GetWindowHandle().get());

  // Set callbacks
  glfwSetMouseButtonCallback(window, MouseButtonCallBack);
  glfwSetCursorPosCallback(window, MouseCursorPositionCallback);
}

void Terminate() {

}
} /* namespace EventHandler*/
} /* namespace Particle */
} /* namespace Gem */
