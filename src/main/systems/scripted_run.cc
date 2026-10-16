/*************************************************************************
 * Copyright (c) 2026 François Trudel
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
// Scripted runs (e.g. system tests): exit after a number of frames, optionally saving the last one
//C system files
//C++ system files
#include <filesystem>
#include <stdexcept>
#include <string>
//Other libraries' .h files
#include <nlohmann/json.hpp>
//Your project's .h files
#include "main/app_registry.hh"
#include "utils/graphic_contexts/graphic_context.hh"

namespace gem {
namespace particle {
namespace app {
namespace {
struct ScriptedRun {
  long long   frames = 0;
  std::string screenshot;  // PNG of the last frame; empty for none
  long long   frame  = 0;  // Frames run so far
};

void from_json(const nlohmann::json& a_Entry, ScriptedRun& a_rComponent) {
  a_Entry.at("frames").get_to(a_rComponent.frames);
  if (a_rComponent.frames <= 0) {
    throw std::runtime_error("ScriptedRun: frames must be positive");
  }
  // Relative to where the app was launched from, since it then moves to its own folder
  const std::string screenshot = a_Entry.value("screenshot", std::string());
  a_rComponent.screenshot = screenshot.empty() ? screenshot : std::filesystem::absolute(screenshot).string();
}

AppSystem ScriptedRunSystem() {
  AppSystem system;
  system.update = [](AppWorld& a_rWorld) {
    ScriptedRun* run = a_rWorld.components.Get<ScriptedRun>();
    if (run == nullptr || ++run->frame < run->frames) {
      return;
    }
    if (!run->screenshot.empty()) {
      a_rWorld.context.RequestScreenshot(run->screenshot);
    }
    a_rWorld.exitRequested = true;
  };
  return system;
}
}

REGISTER_APP_COMPONENT(ScriptedRun)
REGISTER_APP_SYSTEM(ScriptedRun, ScriptedRunSystem())
} /* namespace app */
} /* namespace particle */
} /* namespace gem */
