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
#include "utils/graphic_contexts/graphic_context_factory.hh"
//C system files
//C++ system files
#include <iostream>
#include <map>
//Other libraries' .h files
//Your project's .h files
#include "utils/graphic_contexts/graphic_context.hh"

namespace gem {
namespace particle {
namespace {
// Function-local, so backends can register from any static initializer
std::map<std::string, GraphicContextConstructor>& Backends() {
  static std::map<std::string, GraphicContextConstructor> backends;
  return backends;
}
}

bool RegisterGraphicContext(const std::string& a_sBackend, GraphicContextConstructor a_fConstruct) {
  return Backends().emplace(a_sBackend, std::move(a_fConstruct)).second;
}

std::unique_ptr<GraphicContext> CreateGraphicContext(const std::string& a_sBackend) {
  const auto backend = Backends().find(a_sBackend);
  if (backend == Backends().end()) {
    std::cerr << __func__ << " -> Unknown graphics backend '" << a_sBackend << "', known ones:";
    for (const auto& known : Backends()) {
      std::cerr << ' ' << known.first;
    }
    std::cerr << std::endl;
    return nullptr;
  }
  return backend->second();
}
} /* namespace particle */
} /* namespace gem */
