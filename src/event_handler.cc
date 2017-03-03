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
#include "event_handler.hh"

#include <iostream>


#ifndef LINUX
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#else
#include <gl/glew.h>
#include <gl/glfw3.h>
#endif

#include "graphic_context.hh"
#include "particle_system.hh"
#include "camera.hh"
#include "shader.hh"
#include "timer.hh"

namespace Gem {
namespace Particle {
namespace EventHandler {
namespace {
// TODO: If it's worth it, move these settings someplace else
// Camera settings
glm::vec3 camera_direction;
float camera_speed;
float mouse_sensitivity;
double yaw;
double pitch;
double last_x;
double last_y;

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
        // Reset mouse to center of screen
        glfwSetCursorPos(a_pWindow, last_x, last_y);
      }
      else {
        mouse_state = FREE_CURSOR;
      }
      break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      break;
    default:
      break;
  }
}
bool firstMouse = true;
void MouseCursorPositionCallback(GLFWwindow* a_pWindow, double a_dXPos, double a_dYPos) {
  if (mouse_state == CAMERA_MOVING) {
    /*
     * Reference: https://learnopengl.com/index.php#!Getting-started/Camera
     */
    if (firstMouse)
    {
      last_x = a_dXPos;
      last_y = a_dYPos;
      firstMouse = false;
    }

    double xoffset = a_dXPos - last_x;
    double yoffset = last_y - a_dYPos; // Reversed since y-coordinates go from bottom to left
    last_x = a_dXPos;
    last_y = a_dYPos;

    double sensitivity = 0.05;	// Change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    // TODO: Create constants for these magical numbers/values
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;

    glm::vec3 front = {
      cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
      sin(glm::radians(pitch)),
      sin(glm::radians(yaw)) * cos(glm::radians(pitch)) 
    };

    camera_direction = glm::normalize(front);
    auto  position = Camera::GetEyePosition(),
          up = Camera::GetUpVector();
    Camera::LookAt(position, position + camera_direction, up);
  }
}

void KeyCallback(GLFWwindow* a_pWindow,  int a_nKeyID, int a_nScanCode, int a_nAction, int n_aMods) {
  if (a_nAction == GLFW_PRESS || a_nAction == GLFW_REPEAT) {
  auto position       = Camera::GetEyePosition();
  auto targetPosition = Camera::GetTargetPosition();
  auto up = Camera::GetUpVector();
  auto camera_right = glm::normalize(glm::cross(camera_direction, up)) * camera_speed;
    switch(a_nKeyID) {
      // Move forward
      case GLFW_KEY_W:
        position += camera_direction * camera_speed;
        Camera::SetEyePosition(position);
        break;
      // Move backward
      case GLFW_KEY_S:
        position -= camera_direction * camera_speed;
        Camera::SetEyePosition(position);
        break;
      // Move right
      case GLFW_KEY_D:
        position += camera_right;
        targetPosition += camera_right;
        Camera::LookAt(position,targetPosition,Camera::GetUpVector());
        break;
      // Move left
      case GLFW_KEY_A:
        position -= camera_right;
        targetPosition -= camera_right;
        Camera::LookAt(position, targetPosition, Camera::GetUpVector());
        break;
      default:
        break;
    }
  }
}
}

void Init(const std::shared_ptr<GraphicContext>& a_pCtxt) {
  // TODO: If it's worth it, move these hardcoded values someplace else
  yaw = -90.0f;
  pitch = 0.0f;
  last_x = 0.0f;
  last_y = 0.0f;

  camera_speed = 0.05f;
  camera_direction = Camera::GetTargetPosition() - Camera::GetEyePosition();
  mouse_sensitivity = 0.005f;

  context_handle = a_pCtxt;
  mouse_state = FREE_CURSOR;
  GLFWwindow* window = static_cast<GLFWwindow*>(context_handle->GetWindowHandle());

  // Set callbacks
  glfwSetMouseButtonCallback(window, MouseButtonCallBack);
  glfwSetCursorPosCallback(window, MouseCursorPositionCallback);
  glfwSetKeyCallback(window, KeyCallback);
}

void Terminate() {

}
} /* namespace EventHandler*/
} /* namespace Particle */
} /* namespace Gem */
