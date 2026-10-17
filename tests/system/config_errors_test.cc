// Invalid configs and arguments stop the app with a reason, before any window opens.
// Run without a display: an app trying to open a window would fail with GLFW's error instead.
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "system/system_test_helpers.hh"

namespace gem {
namespace particle {
namespace system_test {
namespace {
using testing::HasSubstr;

const char* NO_DISPLAY = "env -u DISPLAY -u WAYLAND_DISPLAY";
const int UNRECOGNIZED_PROJECT = 0x01;  // app::ErrCode
const int CONFIG_ERROR         = 0x06;

AppRun RunWithoutDisplay(const nlohmann::json& a_Config, const std::vector<std::string>& a_Arguments = {}) {
  return RunApp(TestDirectory(), a_Config, a_Arguments, NO_DISPLAY);
}

TEST(ConfigErrorsTest, UnknownDefaultProject) {
  nlohmann::json config = BaseConfig();
  config["projects"]["default"] = "volcano";
  const AppRun run = RunWithoutDisplay(config);
  EXPECT_EQ(run.exitCode, UNRECOGNIZED_PROJECT) << run.output;
  EXPECT_THAT(run.output, HasSubstr("volcano"));
}

TEST(ConfigErrorsTest, UnknownProjectArgument) {
  const AppRun run = RunWithoutDisplay(BaseConfig(), { "volcano" });
  EXPECT_NE(run.exitCode, 0) << run.output;
  EXPECT_THAT(run.output, HasSubstr("volcano"));
}

TEST(ConfigErrorsTest, UnknownBackend) {
  nlohmann::json config = BaseConfig();
  config["graphics"]["backend"] = "vulkan";
  const AppRun run = RunWithoutDisplay(config);
  EXPECT_EQ(run.exitCode, CONFIG_ERROR) << run.output;
  EXPECT_THAT(run.output, HasSubstr("vulkan"));
}

TEST(ConfigErrorsTest, UnknownComponentType) {
  nlohmann::json config = BaseConfig();
  config["components"].push_back({ { "type", "Teleporter" } });
  const AppRun run = RunWithoutDisplay(config);
  EXPECT_EQ(run.exitCode, CONFIG_ERROR) << run.output;
  EXPECT_THAT(run.output, HasSubstr("Teleporter"));
}

TEST(ConfigErrorsTest, UnknownKeyName) {
  nlohmann::json config = BaseConfig();
  config["components"].push_back({ { "type", "NextProjectKey" }, { "key", "NoSuchKey" } });
  const AppRun run = RunWithoutDisplay(config);
  EXPECT_EQ(run.exitCode, CONFIG_ERROR) << run.output;
  EXPECT_THAT(run.output, HasSubstr("NoSuchKey"));
}

TEST(ConfigErrorsTest, ScriptedRunWithoutFrames) {
  nlohmann::json config = BaseConfig();
  config["components"].push_back({ { "type", "ScriptedRun" } });
  const AppRun run = RunWithoutDisplay(config);
  EXPECT_EQ(run.exitCode, CONFIG_ERROR) << run.output;
  EXPECT_THAT(run.output, HasSubstr("frames"));
}

TEST(ConfigErrorsTest, MissingSetting) {
  nlohmann::json config = BaseConfig();
  config.erase("window");
  const AppRun run = RunWithoutDisplay(config);
  EXPECT_EQ(run.exitCode, CONFIG_ERROR) << run.output;
  EXPECT_THAT(run.output, HasSubstr("window"));
}

TEST(ConfigErrorsTest, MissingConfigFile) {
  const AppRun run = RunApp(TestDirectory(), nullptr, { "--config", "/nonexistent/config.json" }, NO_DISPLAY);
  EXPECT_NE(run.exitCode, 0) << run.output;
  EXPECT_THAT(run.output, HasSubstr("/nonexistent/config.json"));
}
} /* namespace */
} /* namespace system_test */
} /* namespace particle */
} /* namespace gem */
