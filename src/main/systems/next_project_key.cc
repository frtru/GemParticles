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
// A global key switching to the next project
//C system files
//C++ system files
#include <stdexcept>
#include <string>
//Other libraries' .h files
#include <nlohmann/json.hpp>
#include "imgui.h"
//Your project's .h files
#include "main/app_registry.hh"

namespace gem {
namespace particle {
namespace app {
namespace {
struct NextProjectKey {
  ImGuiKey key = ImGuiKey_None;
};

// Key from its ImGui name ("N", "Tab", "F5", ...), ImGuiKey_None when the name is unknown
ImGuiKey KeyFromName(const std::string& a_sName) {
  for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key) {
    if (a_sName == ImGui::GetKeyName(static_cast<ImGuiKey>(key))) {
      return static_cast<ImGuiKey>(key);
    }
  }
  return ImGuiKey_None;
}

void from_json(const nlohmann::json& a_Entry, NextProjectKey& a_rComponent) {
  const std::string name = a_Entry.at("key").get<std::string>();
  a_rComponent.key = KeyFromName(name);
  if (a_rComponent.key == ImGuiKey_None) {
    throw std::runtime_error("NextProjectKey: unknown key '" + name + "', expected an ImGui key name such as \"N\" or \"F5\"");
  }
}

AppSystem NextProjectKeySystem() {
  AppSystem system;
  system.update = [](AppWorld& a_rWorld) {
    const NextProjectKey* component = a_rWorld.components.Get<NextProjectKey>();
    if (component != nullptr && !ImGui::GetIO().WantCaptureKeyboard &&
        ImGui::IsKeyPressed(component->key, /*repeat=*/false)) {
      a_rWorld.nextProjectRequested = true;
    }
  };
  return system;
}
}

REGISTER_APP_COMPONENT(NextProjectKey)
REGISTER_APP_SYSTEM(NextProjectKey, NextProjectKeySystem())
} /* namespace app */
} /* namespace particle */
} /* namespace gem */
