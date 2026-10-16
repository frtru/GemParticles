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
#ifndef APP_WORLD_HH
#define APP_WORLD_HH

#include <any>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace gem {
namespace particle {
class GraphicContext;

namespace app {
// The app's components: plain data declared in the config, at most one of each type
class ComponentStore {
public:
  template <class Component>
  void Add(Component a_Component) {
    m_Components[typeid(Component)] = std::move(a_Component);
  }

  // nullptr when the app has no such component
  template <class Component>
  Component* Get() {
    const auto component = m_Components.find(typeid(Component));
    return component != m_Components.end() ? std::any_cast<Component>(&component->second) : nullptr;
  }

private:
  std::unordered_map<std::type_index, std::any> m_Components;
};

// What the app's systems read and write
struct AppWorld {
  ComponentStore  components;
  GraphicContext& context;
  std::string     project;  // The running project
  // Requests are data too: the app applies them between frames
  bool            nextProjectRequested = false;
  bool            exitRequested        = false;
};
} /* namespace app */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: APP_WORLD_HH */
