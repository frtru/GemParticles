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
#include "projects/light/light_event_handler.hh"

#include <iostream>
#include <mutex>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <AntTweakBar.h>

#include "graphic_contexts/graphic_context.hh"
#include "core/particle_module.hh"
#include "utils/timer.hh"
#include "utils/camera.hh"
#include "utils/light_module.hh"
#include <glm/gtc/matrix_transform.inl>

#include "projects/light/scene_with_light.hh"

namespace gem { namespace particle {
namespace light_project {
namespace event_handler {
namespace {

std::once_flag init_flag;
std::once_flag terminate_flag;

// TODO: If it's worth it, move these settings someplace else
// Window dimension
int _WindowWidth  = 640;
int _WindowHeight = 480;

// Camera settings
glm::vec3 camera_direction;
float camera_speed;
float mouse_sensitivity;
double yaw;
double pitch;
double last_x;
double last_y;

enum MouseState {
  POI_MOVING,
  CAMERA_MOVING,
  FREE_CURSOR
}; 
MouseState                      mouse_state;
bool firstMouse = true;

// Handles
std::shared_ptr<GraphicContext>         context_handle;

void MouseButtonCallBack(GLFWwindow* a_pWindow, int a_nButtonID, int a_nAction, int a_nMods) {
  switch (a_nButtonID) {
  case GLFW_MOUSE_BUTTON_LEFT:
    if (a_nAction == GLFW_PRESS) {
    }
    else {
      mouse_state = FREE_CURSOR;
      firstMouse = true;
    }
    break;
  case GLFW_MOUSE_BUTTON_MIDDLE:
    break;
  case GLFW_MOUSE_BUTTON_RIGHT:
    if (a_nAction == GLFW_PRESS) {
      mouse_state = CAMERA_MOVING;
    }
    else {
      mouse_state = FREE_CURSOR;
      firstMouse = true;
    }
    break;
  default:
    break;
  }
}

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
      return;
    }

    double xoffset = a_dXPos - last_x;
    double yoffset = last_y - a_dYPos; // Reversed since y-coordinates go from bottom to left
    last_x = a_dXPos;
    last_y = a_dYPos;

    double sensitivity = 0.10;	// Change this value to your liking
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
    auto  position = camera::GetEyePosition(),
      up = camera::GetUpVector();
    camera::LookAt(position, position + camera_direction, up);
  }
  //else if (mouse_state == POI_MOVING) {
  //  glm::mat4 P = camera::GetProjectionMatrix();
  //  glm::mat4 V = camera::GetViewMatrix();

  //  glm::vec3 from = glm::unProject(
  //    glm::vec3(a_dXPos, _WindowHeight - a_dYPos, 0.0f), V, P,
  //    glm::vec4(0, 0, _WindowWidth, _WindowHeight));
  //  glm::vec3 to = glm::unProject(
  //    glm::vec3(a_dXPos, _WindowHeight - a_dYPos, 1.0f), V, P,
  //    glm::vec4(0, 0, _WindowWidth, _WindowHeight));

  //  const glm::f32vec3 wCameraPos = camera::GetEyePosition();

  //  glm::f32vec3 wNewPos = from + glm::normalize((to - from)) *
  //    glm::distance(wCameraPos, glm::f32vec3(0.0f, 0.0f, 0.0f));
  //  _AttractorHandle->SetAttractorPosition(wNewPos);
  //  _ColorUpdaterHandle->SetPOI(wNewPos);
  //}
  else {
    // DO NOTHING
  }
}

void KeyCallback(GLFWwindow* a_pWindow,  int a_nKeyID, int a_nScanCode, int a_nAction, int n_aMods) {
  if (a_nAction == GLFW_PRESS || a_nAction == GLFW_REPEAT) {
  auto position       = camera::GetEyePosition();
  auto targetPosition = camera::GetTargetPosition();
  auto up = camera::GetUpVector();
  auto camera_right = glm::normalize(glm::cross(camera_direction, up)) * camera_speed;
  auto camera_forward = camera_direction * camera_speed;
  constexpr std::size_t lightNumber = 1U;
  light::Light wLight = light::module::GetLight(lightNumber);
    switch(a_nKeyID) {
      // Move forward
      case GLFW_KEY_W:
        position += camera_forward;
        targetPosition += camera_forward;
        camera::LookAt(position, targetPosition, camera::GetUpVector());
        break;
      // Move backward
      case GLFW_KEY_S:
        position -= camera_forward;
        targetPosition -= camera_forward;
        camera::LookAt(position, targetPosition, camera::GetUpVector());
        break;
      // Move right
      case GLFW_KEY_D:
        position += camera_right;
        targetPosition += camera_right;
        camera::LookAt(position,targetPosition,camera::GetUpVector());
        break;
      // Move left
      case GLFW_KEY_A:
        position -= camera_right;
        targetPosition -= camera_right;
        camera::LookAt(position, targetPosition, camera::GetUpVector());
        break;
        // Toggle debug option
      case GLFW_KEY_B:
        scene::SetDebugOption(!scene::IsDebug());
        break;
      case GLFW_KEY_SPACE:
        camera::LookAt(position, { 0.0f,0.0f,0.0f }, camera::GetUpVector());
        break;
      case GLFW_KEY_U:
        wLight.position += glm::f32vec3(0.1f,0.0f,0.0f);
        light::module::UpdateLight(lightNumber, wLight);
        break;
      case GLFW_KEY_I:
        wLight.position += glm::f32vec3(0.0f, 0.1f, 0.0f);
        light::module::UpdateLight(lightNumber, wLight);
        break;
      case GLFW_KEY_O:
        wLight.position += glm::f32vec3(0.0f, 0.0f, 0.1f);
        light::module::UpdateLight(lightNumber, wLight);
        break;
      case GLFW_KEY_J:
        wLight.position -= glm::f32vec3(0.1f, 0.0f, 0.0f);
        light::module::UpdateLight(lightNumber, wLight);
        break;
      case GLFW_KEY_K:
        wLight.position -= glm::f32vec3(0.0f, 0.1f, 0.0f);
        light::module::UpdateLight(lightNumber, wLight);
        break;
      case GLFW_KEY_L:
        wLight.position -= glm::f32vec3(0.0f, 0.0f, 0.1f);
        light::module::UpdateLight(lightNumber, wLight);
        break;
      default:
        break;
    }
  }
}

void FramebufferSizeCallback(GLFWwindow* a_pWindow, int a_nWidth, int a_nHeight) {
  _WindowWidth = a_nWidth;
  _WindowHeight = a_nHeight;
  glViewport(0, 0, a_nWidth, a_nHeight);
  camera::SetPerspectiveProjection(
    glm::radians(45.0f),
    a_nWidth, a_nHeight,
    0.1f, 100.0f);
}
}

void Init(const std::shared_ptr<GraphicContext>& a_pCtxt) {
  std::call_once(init_flag, [&]() {
    // Get a reference on the dynamics of this project
    //_AttractorHandle = a_pAttractorHandle;
    //_ColorUpdaterHandle = a_pColorUpdater;

    // TODO: If it's worth it, move these hardcoded values someplace else
    yaw = -90.0f;
    pitch = 0.0f;
    last_x = 0.0f;
    last_y = 0.0f;

    camera_speed = 0.05f;
    camera_direction = camera::GetTargetPosition() - camera::GetEyePosition();
    mouse_sensitivity = 0.005f;

    context_handle = a_pCtxt;
    mouse_state = FREE_CURSOR;
    GLFWwindow* window = static_cast<GLFWwindow*>(context_handle->GetWindowHandle());

    // Set callbacks
    glfwSetMouseButtonCallback(window, MouseButtonCallBack);
    glfwSetCursorPosCallback(window, MouseCursorPositionCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
  });
}

void Update() { 
}
} /* namespace event_handler*/
} /* namespace light_project */
} /* namespace particle */
} /* namespace gem */
