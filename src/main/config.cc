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
#include "config.hh"
//C system files
//C++ system files
#include <fstream>
#include <iostream>
#include <stdexcept>
//Other libraries' .h files
#include <nlohmann/json.hpp>
//Your project's .h files
#include "main/app_registry.hh"

namespace gem {
namespace particle {
namespace app {
std::optional<Config> LoadConfig(const std::string& a_sPath) {
  try {
    std::ifstream file(a_sPath);
    if (!file) {
      throw std::runtime_error("cannot open the file");
    }
    const nlohmann::json json = nlohmann::json::parse(file, nullptr, true, /*ignore_comments=*/true);

    Config config;
    json.at("graphics").at("backend").get_to(config.backend);
    json.at("window").at("width").get_to(config.window.width);
    json.at("window").at("height").get_to(config.window.height);
    json.at("window").at("fullscreen").get_to(config.window.fullscreen);
    json.at("projects").at("default").get_to(config.defaultProject);
    for (const nlohmann::json& component : json.at("components")) {
      AddComponent(component, config.components);
    }
    if (config.window.width <= 0 || config.window.height <= 0) {
      throw std::runtime_error("the window size must be positive");
    }
    return config;
  }
  catch (const std::exception& error) {
    std::cerr << __func__ << " -> " << a_sPath << ": " << error.what() << std::endl;
    return std::nullopt;
  }
}
} /* namespace app */
} /* namespace particle */
} /* namespace gem */
