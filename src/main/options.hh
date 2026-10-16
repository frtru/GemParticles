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
#ifndef OPTIONS_HH
#define OPTIONS_HH

#include <optional>
#include <string>

#include "utils/graphic_contexts/graphic_context.hh"

namespace gem {
namespace particle {
namespace app {
struct Options {
  std::string     project;
  ContextOptions  context;
};

// Fills a_rOptions from the command line. Returns the exit code to stop with
// when the app must not start (--help, invalid arguments), nothing otherwise.
std::optional<int> ParseOptions(int argc, const char* argv[], Options& a_rOptions);
} /* namespace app */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: OPTIONS_HH */
