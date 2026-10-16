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
#include "app_registry.hh"
//C system files
//C++ system files
#include <stdexcept>
//Other libraries' .h files
#include <nlohmann/json.hpp>
//Your project's .h files

namespace gem {
namespace particle {
namespace app {
namespace {
// Function-local, so registrations work from any static initializer
std::map<std::string, ComponentCreator>& ComponentTypes() {
  static std::map<std::string, ComponentCreator> types;
  return types;
}

std::map<std::string, AppSystem>& Systems() {
  static std::map<std::string, AppSystem> systems;
  return systems;
}
}

bool RegisterComponentType(const std::string& a_sType, ComponentCreator a_fCreate) {
  return ComponentTypes().emplace(a_sType, std::move(a_fCreate)).second;
}

void AddComponent(const nlohmann::json& a_Entry, ComponentStore& a_rStore) {
  const std::string type = a_Entry.at("type").get<std::string>();
  const auto creator = ComponentTypes().find(type);
  if (creator == ComponentTypes().end()) {
    std::string known;
    for (const auto& entry : ComponentTypes()) {
      known += ' ' + entry.first;
    }
    throw std::runtime_error("unknown component type '" + type + "', known ones:" + known);
  }
  creator->second(a_Entry, a_rStore);
}

bool RegisterAppSystem(const std::string& a_sName, AppSystem a_System) {
  return Systems().emplace(a_sName, std::move(a_System)).second;
}

const std::map<std::string, AppSystem>& AppSystems() {
  return Systems();
}
} /* namespace app */
} /* namespace particle */
} /* namespace gem */
