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
#ifndef GRAPHIC_CONTEXT_HH
#define GRAPHIC_CONTEXT_HH

#include <functional>
#include <memory>
#include <string>

#include "utils/macro_definitions.hh"

namespace gem {
namespace particle {
// Launch settings shared by every context. The app sets them once,
// before a project creates its context.
struct ContextOptions {
  bool        fullscreen = false;
  long long   maxFrames  = -1;  // < 0: run until the window is closed
  std::string screenshotPath;   // PNG written after the last frame when set
};

// Input events forwarded by the context. Key, button and action codes are GLFW's.
// Events ImGui wants (clicks over its windows, typing in its fields) are not forwarded.
struct InputCallbacks {
  std::function<void(int a_nButton, int a_nAction, int a_nMods)>               mouseButton;
  std::function<void(double a_dX, double a_dY)>                                cursorPosition;
  std::function<void(int a_nKey, int a_nScanCode, int a_nAction, int a_nMods)> key;
  std::function<void(int a_nWidth, int a_nHeight)>                             resize;
};

class GraphicContext {
  DECLARE_UNCOPYABLE(GraphicContext)
  DECLARE_UNMOVABLE(GraphicContext)
public:
  GraphicContext() {}
  virtual ~GraphicContext() {}

  virtual void NewFrame() = 0;
  virtual void Update() = 0;
  virtual bool PollWindowClosedEvent() = 0;
  virtual void Reshape(int a_width, int a_height) = 0;

  void Init();
  void Terminate();

  static void SetOptions(const ContextOptions& a_Options) { m_Options = a_Options; }
  static const ContextOptions& GetOptions() { return m_Options; }

  void SetInputCallbacks(InputCallbacks a_Callbacks) { m_InputCallbacks = std::move(a_Callbacks); }

  virtual void* GetWindowHandle() const = 0;
  virtual std::size_t GetWindowWidth() const = 0;
  virtual std::size_t GetWindowHeight() const = 0;

protected:
  const InputCallbacks& GetInputCallbacks() const { return m_InputCallbacks; }

private:
  virtual void InitImpl() = 0;
  virtual void TerminateImpl() = 0;

  static bool  m_bInitialized;
  static bool  m_bTerminated;
  inline static ContextOptions m_Options;
  InputCallbacks m_InputCallbacks;
}; /* class GraphicContext*/
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: GRAPHIC_CONTEXT_HH */
