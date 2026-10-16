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
// Debug windows: stats, property editor and log
//C system files
//C++ system files
//Other libraries' .h files
#include <nlohmann/json.hpp>
#include "imgui.h"
//Your project's .h files
#include "main/app_registry.hh"
#include "core/particle_module.hh"
#include "utils/imgui/imgui_log.h"
#include "utils/imgui/imgui_property_editor.h"

namespace gem {
namespace particle {
namespace app {
namespace {
struct DebugOverlay {};
void from_json(const nlohmann::json&, DebugOverlay&) {}

void DrawWindows(AppWorld& a_rWorld) {
  if (a_rWorld.components.Get<DebugOverlay>() == nullptr) {
    return;
  }
  ImGui::Begin("Stats");
  ImGui::Text("Project: %s", a_rWorld.project.c_str());
  ImGui::Text("Application average %.3f ms / frame(%.1f FPS)\n", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::Text("Total active particles = %zu\n", particle_module::GetActiveParticlesCount());
  ImGui::End();
  ImGuiPropertyEditor::GetInstance().Draw("Property editor");
  ImGuiLog::GetInstance().Draw("Debugging logs");
}

AppSystem DebugOverlaySystem() {
  AppSystem system;
  system.onProjectStarted = [](AppWorld& a_rWorld) {
    ImGuiLog::GetInstance().AddLog("--- %s ---\n", a_rWorld.project.c_str());
  };
  system.update = DrawWindows;
  // The project's components registered their properties: they point into the stopped project
  system.onProjectStopped = [](AppWorld&) { ImGuiPropertyEditor::GetInstance().Clear(); };
  return system;
}
}

REGISTER_APP_COMPONENT(DebugOverlay)
REGISTER_APP_SYSTEM(DebugOverlay, DebugOverlaySystem())
} /* namespace app */
} /* namespace particle */
} /* namespace gem */
