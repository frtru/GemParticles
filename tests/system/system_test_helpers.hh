#ifndef SYSTEM_TEST_HELPERS_HH
#define SYSTEM_TEST_HELPERS_HH

#include <sys/wait.h>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

namespace gem {
namespace particle {
namespace system_test {
struct AppRun {
  int         exitCode = -1;
  std::string output;  // stdout and stderr
};

// A folder of the system's temp directory, emptied, for the running test
inline std::filesystem::path TestDirectory() {
  const testing::TestInfo* test = testing::UnitTest::GetInstance()->current_test_info();
  std::string name = std::string(test->test_suite_name()) + "." + test->name();
  for (char& c : name) {
    if (c == '/') c = '_';
  }
  const std::filesystem::path directory = std::filesystem::temp_directory_path() / "gem_system_tests" / name;
  std::filesystem::remove_all(directory);
  std::filesystem::create_directories(directory);
  return directory;
}

inline bool HasDisplay() {
  return std::getenv("DISPLAY") != nullptr || std::getenv("WAYLAND_DISPLAY") != nullptr;
}

// Settings every system test starts from: a small window, no debug windows or hotkey
inline nlohmann::json BaseConfig() {
  return nlohmann::json::parse(R"({
    "graphics":   { "backend": "opengl" },
    "window":     { "width": 640, "height": 360, "fullscreen": false },
    "projects":   { "default": "attractor" },
    "components": []
  })");
}

// Runs the app from a_rDirectory with the extra arguments and the config, written in a_rDirectory
// (no --config for a null one). a_sEnvironment prefixes the command, e.g. "env -u DISPLAY".
inline AppRun RunApp(const std::filesystem::path& a_rDirectory, const nlohmann::json& a_Config,
  const std::vector<std::string>& a_Arguments = {}, const std::string& a_sEnvironment = "") {
  std::string command = "cd '" + a_rDirectory.string() + "' && " + a_sEnvironment + " '" GEM_APP_PATH "'";
  for (const std::string& argument : a_Arguments) {
    command += " '" + argument + "'";
  }
  if (!a_Config.is_null()) {
    const std::filesystem::path config = a_rDirectory / "config.json";
    std::ofstream(config) << a_Config.dump(2);
    command += " --config '" + config.string() + "'";
  }
  command += " 2>&1";

  AppRun run;
  FILE* pipe = popen(command.c_str(), "r");
  if (pipe == nullptr) {
    return run;
  }
  char buffer[4096];
  while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
    run.output += buffer;
  }
  const int status = pclose(pipe);
  run.exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
  return run;
}
} /* namespace system_test */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: SYSTEM_TEST_HELPERS_HH */
