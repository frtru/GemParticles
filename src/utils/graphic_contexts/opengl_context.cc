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
#include "utils/graphic_contexts/opengl_context.hh"
#include "utils/graphic_contexts/graphic_context_factory.hh"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "utils/imgui/imgui_log.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <iostream>
#include <unordered_set>
#include <vector>

namespace gem {
namespace particle {
namespace {
#ifndef NDEBUG
void GLAPIENTRY GLDebugCallback(GLenum a_eSource, GLenum a_eType, GLuint a_unID,
  GLenum a_eSeverity, GLsizei a_nLength, const GLchar* a_sMessage, const void* a_pUserParam) {
  // Performance warnings repeat every frame (e.g. per-frame buffer uploads): report each once
  static std::unordered_set<GLuint> s_ReportedPerformanceIDs;
  if (a_eType == GL_DEBUG_TYPE_PERFORMANCE && !s_ReportedPerformanceIDs.insert(a_unID).second) {
    return;
  }
  const char* severity = a_eSeverity == GL_DEBUG_SEVERITY_HIGH   ? "HIGH" :
                         a_eSeverity == GL_DEBUG_SEVERITY_MEDIUM ? "MEDIUM" : "LOW";
  ImGuiLog::GetInstance().AddLog("[GL %s] %s\n", severity, a_sMessage);
}
#endif

OpenGLContext& ContextOf(GLFWwindow* a_pWindow) {
  return *static_cast<OpenGLContext*>(glfwGetWindowUserPointer(a_pWindow));
}
}

REGISTER_GRAPHIC_CONTEXT("opengl", OpenGLContext)

OpenGLContext::OpenGLContext() 
  : m_pWindow(nullptr) {}

OpenGLContext::~OpenGLContext() {
  // TODO: When closing window first instead of console
  // it tries to delete the GLFWwindow* which is already
  // deleted, maybe the shaded_ptr isnt a good idea
}

void OpenGLContext::RequestScreenshot(const std::string& a_sPath) {
  m_sScreenshotPath = a_sPath;
}

void OpenGLContext::ResetRenderState() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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

void OpenGLContext::NewFrame() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void OpenGLContext::Update() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (!m_sScreenshotPath.empty()) {
    SaveScreenshot(m_sScreenshotPath);
    m_sScreenshotPath.clear();
  }

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

void OpenGLContext::SaveScreenshot(const std::string& a_sPath) const {
  // Reads the back buffer (scene + ImGui) before it gets swapped
  int width = 0, height = 0;
  glfwGetFramebufferSize(m_pWindow, &width, &height);
  std::vector<unsigned char> pixels(static_cast<std::size_t>(width) * height * 3);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadBuffer(GL_BACK);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

  stbi_flip_vertically_on_write(1);
  if (stbi_write_png(a_sPath.c_str(), width, height, 3, pixels.data(), width * 3)) {
    std::cout << "OpenGLContext::SaveScreenshot -> Saved " << width << "x" << height
              << " screenshot to " << a_sPath << std::endl;
  }
  else {
    std::cerr << "OpenGLContext::SaveScreenshot -> Failed to write " << a_sPath << std::endl;
  }
}

void OpenGLContext::Reshape(int a_width, int a_height) {
  glViewport(0, 0, a_width, a_height);
}

void OpenGLContext::InstallInputCallbacks() {
  // Installed before ImGui's backend, which chains to them: ImGui sees each event first,
  // so the ones it wants can be kept away from the project's callbacks
  glfwSetWindowUserPointer(m_pWindow, this);
  glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* a_pWindow, int a_nButton, int a_nAction, int a_nMods) {
    // Releases always go through so a drag started outside ImGui can't get stuck
    if (a_nAction == GLFW_PRESS && ImGui::GetIO().WantCaptureMouse) {
      return;
    }
    if (const auto& callback = ContextOf(a_pWindow).GetInputCallbacks().mouseButton) {
      callback(a_nButton, a_nAction, a_nMods);
    }
  });
  glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* a_pWindow, double a_dX, double a_dY) {
    if (const auto& callback = ContextOf(a_pWindow).GetInputCallbacks().cursorPosition) {
      callback(a_dX, a_dY);
    }
  });
  glfwSetKeyCallback(m_pWindow, [](GLFWwindow* a_pWindow, int a_nKey, int a_nScanCode, int a_nAction, int a_nMods) {
    if (ImGui::GetIO().WantCaptureKeyboard) {
      return;
    }
    if (const auto& callback = ContextOf(a_pWindow).GetInputCallbacks().key) {
      callback(a_nKey, a_nScanCode, a_nAction, a_nMods);
    }
  });
  glfwSetFramebufferSizeCallback(m_pWindow, [](GLFWwindow* a_pWindow, int a_nWidth, int a_nHeight) {
    OpenGLContext& context = ContextOf(a_pWindow);
    context.Reshape(a_nWidth, a_nHeight);
    if (const auto& callback = context.GetInputCallbacks().resize) {
      callback(a_nWidth, a_nHeight);
    }
  });
}

void OpenGLContext::InitImpl(const WindowOptions& a_Options) {
  // Provide a way to set windowed mode or not, size and other useful parameters

  // GLFW initialization
  /* Initialize the library */
  if (!glfwInit())
    std::cerr << "OpenGLSetup -> glfwInit failed!" << std::endl;
  
  // Modern OpenGL
  // NOTE: MUST BE COMMENTED-OUT IN ORDER TO USE ANTTWEAKBAR
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

  /* Create the window (windowed by default, fullscreen on request) and its OpenGL context */
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  if (a_Options.fullscreen) {
    m_pWindow = glfwCreateWindow(mode->width, mode->height, "GemParticles", monitor, NULL);
  }
  else {
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    m_pWindow = glfwCreateWindow(a_Options.width, a_Options.height, "GemParticles", NULL, NULL);
  }
  if (!m_pWindow) {
    glfwTerminate();
    std::cerr << "OpenGLSetup -> glfwCreateWindow failed!" << std::endl;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(m_pWindow);
  glfwSwapInterval(0);

  /* Ensure we can capture keys being pressed */
  glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
  //glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  
  //TODO: Insert other glfw parameters here

  // GLEW initialization (experimental is required to load every entry point in a core profile)
  glewExperimental = GL_TRUE;
  if (GLEW_OK != glewInit()) {
    std::cerr << "GLEW is not initialized!" << std::endl;
  }
  // glewInit queries extensions the legacy way, which raises a harmless GL_INVALID_ENUM in core profile
  glGetError();
  std::cout << "OpenGLContext::InitImpl -> " << glGetString(GL_VENDOR) << " | "
            << glGetString(GL_RENDERER) << " | " << glGetString(GL_VERSION) << std::endl;

#ifndef NDEBUG
  // Debug output is core since GL 4.3, the version requested above
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Report on the offending call's stack
  glDebugMessageCallback(GLDebugCallback, nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

  // OpenGL initialization
  ResetRenderState();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  int framebufferWidth = 0, framebufferHeight = 0;
  glfwGetFramebufferSize(m_pWindow, &framebufferWidth, &framebufferHeight);
  Reshape(framebufferWidth, framebufferHeight);

  InstallInputCallbacks();

  // ImGui initialization
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
  ImGui_ImplOpenGL3_Init();
  ImGui::StyleColorsClassic();
  glfwSetWindowTitle(m_pWindow, "GemParticles");
}

void OpenGLContext::TerminateImpl() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  std::cout << "OpenGLContext::TerminateImpl -> Deleting glfw context." << std::endl;
  glfwTerminate();
}
} /* namespace particle */
} /* namespace gem */
