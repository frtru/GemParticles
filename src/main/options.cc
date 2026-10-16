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
#include "options.hh"
//C system files
//C++ system files
#include <filesystem>
#include <vector>
//Other libraries' .h files
#include <CLI/CLI.hpp>
//Your project's .h files
#include "projects/project_dictionary.hh"

namespace gem {
namespace particle {
namespace app {
std::optional<int> ParseOptions(int argc, const char* argv[], Options& a_rOptions) {
  const std::vector<std::string> projects = project_dict::GetProjectNames();

  CLI::App cli{ "GemParticles particle sandbox" };
  cli.add_option("project", a_rOptions.project, "Project to launch (default: the config's)")
    ->check(CLI::IsMember(projects));
  cli.add_option("--config", a_rOptions.configPath, "Config file (default: config.json next to the executable)")
    ->check(CLI::ExistingFile);

  try {
    cli.parse(argc, argv);
  }
  catch (const CLI::ParseError& error) {
    return cli.exit(error);
  }
  // Relative to where the user launched from, since the app moves to its own folder
  if (!a_rOptions.configPath.empty()) {
    a_rOptions.configPath = std::filesystem::absolute(a_rOptions.configPath).string();
  }
  return std::nullopt;
}
} /* namespace app */
} /* namespace particle */
} /* namespace gem */
