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
#ifndef APP_REGISTRY_HH
#define APP_REGISTRY_HH

#include <functional>
#include <map>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include "app_world.hh"

namespace gem {
namespace particle {
namespace app {
// Component types are built from their config entry, e.g. { "type": "NextProjectKey", "key": "N" }
using ComponentCreator = std::function<void(const nlohmann::json& a_Entry, ComponentStore& a_rStore)>;
bool RegisterComponentType(const std::string& a_sType, ComponentCreator a_fCreate);
// Adds the component a config entry describes; throws when its type is unknown
void AddComponent(const nlohmann::json& a_Entry, ComponentStore& a_rStore);

// Behaviour over the components it handles. Every hook is optional.
struct AppSystem {
  std::function<void(AppWorld&)> onProjectStarted;
  std::function<void(AppWorld&)> update;  // Every frame, between the new frame and its presentation
  std::function<void(AppWorld&)> onProjectStopped;
};
bool RegisterAppSystem(const std::string& a_sName, AppSystem a_System);
// Every registered system, in name order
const std::map<std::string, AppSystem>& AppSystems();

// Component types and systems register themselves while the program starts, from their own file.
// A component type needs a from_json(const nlohmann::json&, Type&) next to it.
#define REGISTER_APP_COMPONENT(Type)                                                         \
  namespace {                                                                                \
  [[maybe_unused]] const bool Type##ComponentRegistered = gem::particle::app::RegisterComponentType( \
    #Type, [](const nlohmann::json& a_Entry, gem::particle::app::ComponentStore& a_rStore) {      \
      a_rStore.Add(a_Entry.get<Type>());                                                     \
    });                                                                                      \
  }

#define REGISTER_APP_SYSTEM(Name, system)                                                    \
  namespace {                                                                                \
  [[maybe_unused]] const bool Name##SystemRegistered = gem::particle::app::RegisterAppSystem(#Name, system); \
  }
} /* namespace app */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: APP_REGISTRY_HH */
