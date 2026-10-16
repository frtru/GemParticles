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
#ifndef CONFIG_HH
#define CONFIG_HH

#include <optional>
#include <string>

#include "main/app_world.hh"
#include "utils/graphic_contexts/graphic_context.hh"

namespace gem {
namespace particle {
namespace app {
// App settings and the app's components, read from a config file
struct Config {
  std::string    backend;         // Graphics backend creating the window, e.g. "opengl"
  WindowOptions  window;
  std::string    defaultProject;  // Launched when none is given on the command line
  ComponentStore components;      // From the "components" list
};

// Reads every setting and component from the file. Reports what is wrong on stderr
// and returns nothing when the file, a setting or a component is missing or invalid.
std::optional<Config> LoadConfig(const std::string& a_sPath);
} /* namespace app */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: CONFIG_HH */
