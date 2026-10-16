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
#ifndef GRAPHIC_CONTEXT_FACTORY_HH
#define GRAPHIC_CONTEXT_FACTORY_HH

#include <functional>
#include <memory>
#include <string>

namespace gem {
namespace particle {
class GraphicContext;

using GraphicContextConstructor = std::function<std::unique_ptr<GraphicContext>()>;

// Makes a graphics backend available under the given name (see REGISTER_GRAPHIC_CONTEXT)
bool RegisterGraphicContext(const std::string& a_sBackend, GraphicContextConstructor a_fConstruct);

// Creates the context of the named graphics backend (e.g. "opengl").
// Returns nullptr, after listing the registered backends, when the name is unknown.
std::unique_ptr<GraphicContext> CreateGraphicContext(const std::string& a_sBackend);

// A backend registers itself while the program starts, from its own source file
#define REGISTER_GRAPHIC_CONTEXT(name, Type)                                    \
  namespace {                                                                   \
  [[maybe_unused]] const bool Type##Registered = gem::particle::RegisterGraphicContext(name, []() \
    -> std::unique_ptr<gem::particle::GraphicContext> { return std::make_unique<Type>(); }); \
  }
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: GRAPHIC_CONTEXT_FACTORY_HH */
