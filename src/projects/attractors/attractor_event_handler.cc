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
#include "projects/attractors/attractor_event_handler.hh"

#include <iostream>
#include <mutex>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utils/imgui/imgui_glfw.h"
//#include <AntTweakBar.h>

#include "utils/graphic_contexts/graphic_context.hh"
#include "core/particle_module.hh"
#include "utils/timer.hh"
#include "utils/scene.hh"
#include "utils/camera.hh"
#include "utils/texture_factory.hh"
#include <glm/gtc/matrix_transform.inl>

namespace gem { namespace particle {
namespace attractor_project {
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

struct DataBindings {
  std::size_t _ActiveParticleCount;
  std::size_t _FPS;
  glm::vec4 _PrevColdColor;
  glm::vec4 _PrevHotColor;
} _Bindings;

// Handles
std::shared_ptr<GraphicContext>         context_handle;
std::shared_ptr<ParticleAttractor>      _AttractorHandle;
std::shared_ptr<ProximityColorUpdater>  _ColorUpdaterHandle;

void MouseButtonCallBack(GLFWwindow* a_pWindow, int a_nButtonID, int a_nAction, int a_nMods) {
  //if (!TwEventMouseButtonGLFW(a_nButtonID, a_nAction)) {
  switch (a_nButtonID) {
  case GLFW_MOUSE_BUTTON_LEFT:
    if (a_nAction == GLFW_PRESS) {
      mouse_state = POI_MOVING;
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
  //}
}

void MouseCursorPositionCallback(GLFWwindow* a_pWindow, double a_dXPos, double a_dYPos) {
  //if (!TwEventMousePosGLFW(static_cast<int>(a_dXPos), static_cast<int>(a_dYPos))) {
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
    else if (mouse_state == POI_MOVING) {
      glm::mat4 P = camera::GetProjectionMatrix();
      glm::mat4 V = camera::GetViewMatrix();

      glm::vec3 from = glm::unProject(
        glm::vec3(a_dXPos, _WindowHeight - a_dYPos, 0.0f), V, P,
        glm::vec4(0, 0, _WindowWidth, _WindowHeight));
      glm::vec3 to = glm::unProject(
        glm::vec3(a_dXPos, _WindowHeight - a_dYPos, 1.0f), V, P,
        glm::vec4(0, 0, _WindowWidth, _WindowHeight));

      const glm::f32vec3 wCameraPos = camera::GetEyePosition();

      glm::f32vec3 wNewPos = from + glm::normalize((to - from)) *
        glm::distance(wCameraPos, glm::f32vec3(0.0f, 0.0f, 0.0f));
      _AttractorHandle->SetAttractorPosition(wNewPos);
      _ColorUpdaterHandle->SetPOI(wNewPos);
    }
    else {
      // DO NOTHING
    }
  //}
}

void KeyCallback(GLFWwindow* a_pWindow,  int a_nKeyID, int a_nScanCode, int a_nAction, int n_aMods) {
  ImGui_ImplGlfw_KeyCallback(a_pWindow, a_nKeyID, a_nScanCode, a_nAction, n_aMods);
  if (a_nAction == GLFW_PRESS || a_nAction == GLFW_REPEAT) {
  auto position       = camera::GetEyePosition();
  auto targetPosition = camera::GetTargetPosition();
  auto up = camera::GetUpVector();
  auto camera_right = glm::normalize(glm::cross(camera_direction, up)) * camera_speed;
  auto camera_forward = camera_direction * camera_speed;
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
  //TwWindowSize(a_nWidth, a_nHeight);
}

//void BuildAntTweakBarGUI() {
//  // Set the properties properly
//  TwAddVarRO(_TweakBarGUI, "FPS", TW_TYPE_UINT32, &_TweakBarProperties._FPS, nullptr);
//  TwAddVarRO(_TweakBarGUI, "Active particles", TW_TYPE_UINT32, &_TweakBarProperties._ActiveParticleCount, nullptr);
//
//  // Add variables to AntTweakBar with properties
//  // Particle Attractor
//  TwAddVarRW(_TweakBarGUI, "Attractor Position X", TW_TYPE_FLOAT, &_AttractorHandle->GetAttractorPositionRef()->x, " min=-100 max=100 step=0.2 ");
//  TwAddVarRW(_TweakBarGUI, "Attractor Position Y", TW_TYPE_FLOAT, &_AttractorHandle->GetAttractorPositionRef()->y, " min=-100 max=100 step=0.2 ");
//  TwAddVarRW(_TweakBarGUI, "Attractor Position Z", TW_TYPE_FLOAT, &_AttractorHandle->GetAttractorPositionRef()->z, " min=-100 max=100 step=0.2 ");
//
//  TwAddVarRW(_TweakBarGUI, "Acceleration Rate", TW_TYPE_FLOAT, _AttractorHandle->GetAccelerationRateRef(), " min=-20 max=50 step=0.2 ");
//
//  // Color updater
//  _TweakBarProperties._ColdColor = _ColorUpdaterHandle->GetColdColor();
//  _TweakBarProperties._HotColor = _ColorUpdaterHandle->GetHotColor();
//  _TweakBarProperties._PrevColdColor = _TweakBarProperties._ColdColor;
//  _TweakBarProperties._PrevHotColor = _TweakBarProperties._HotColor;
//  TwAddVarRW(_TweakBarGUI, "Cold Color", TW_TYPE_COLOR32, &_TweakBarProperties._ColdColor, " coloralpha=true ");
//  TwAddVarRW(_TweakBarGUI, "Hot Color", TW_TYPE_COLOR32, &_TweakBarProperties._HotColor, " coloralpha=true ");
//}
}

void Init(const std::shared_ptr<GraphicContext>& a_pCtxt,
  const std::shared_ptr<ParticleAttractor>& a_pAttractorHandle,
  const std::shared_ptr<ProximityColorUpdater>& a_pColorUpdater) {
  std::call_once(init_flag, [&]() {
    // Get a reference on the dynamics of this project
    _AttractorHandle = a_pAttractorHandle;
    _ColorUpdaterHandle = a_pColorUpdater;

    // ImGui initialization
    _Bindings._PrevColdColor = _ColorUpdaterHandle->GetColdColor();
    _Bindings._PrevHotColor = _ColorUpdaterHandle->GetHotColor();

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
    //TwTerminate();
  });
}

void Update() {
  // Update color gradient if needed
  if (_Bindings._PrevColdColor != _ColorUpdaterHandle->GetColdColor() ||
    _Bindings._PrevHotColor != _ColorUpdaterHandle->GetHotColor()) {
    _ColorUpdaterHandle->UpdateColorGradient();
    _Bindings._PrevColdColor = _ColorUpdaterHandle->GetColdColor();
    _Bindings._PrevHotColor = _ColorUpdaterHandle->GetHotColor();
  }
}
} /* namespace event_handler*/
} /* namespace attractor_project */
} /* namespace particle */
} /* namespace gem */
