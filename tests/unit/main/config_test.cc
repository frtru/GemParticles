#include <string>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "main/app_registry.hh"
#include "main/config.hh"
#include "unit/test_doubles.hh"

namespace gem {
namespace particle {
namespace app {
namespace {
using test::WriteTempFile;

nlohmann::json ValidConfig() {
  return nlohmann::json::parse(R"({
    "graphics":   { "backend": "opengl" },
    "window":     { "width": 640, "height": 360, "fullscreen": true },
    "projects":   { "default": "rain" },
    "components": [ { "type": "DebugOverlay" }, { "type": "NextProjectKey", "key": "F5" } ]
  })");
}

std::optional<Config> Load(const nlohmann::json& a_Config) {
  return LoadConfig(WriteTempFile("config.json", a_Config.dump()));
}

TEST(ConfigTest, ReadsEverySetting) {
  const std::optional<Config> config = Load(ValidConfig());
  ASSERT_TRUE(config);
  EXPECT_EQ(config->backend, "opengl");
  EXPECT_EQ(config->window.width, 640);
  EXPECT_EQ(config->window.height, 360);
  EXPECT_TRUE(config->window.fullscreen);
  EXPECT_EQ(config->defaultProject, "rain");
}

struct Marker {};
TEST(ConfigTest, AddsTheListedComponents) {
  RegisterComponentType("TestMarker", [](const nlohmann::json&, ComponentStore& a_rStore) { a_rStore.Add(Marker{}); });
  nlohmann::json json = ValidConfig();
  json["components"].push_back({ { "type", "TestMarker" } });
  std::optional<Config> config = Load(json);
  ASSERT_TRUE(config);
  EXPECT_NE(config->components.Get<Marker>(), nullptr);
}

TEST(ConfigTest, AllowsComments) {
  const std::string text = "// App settings\n" + ValidConfig().dump(2);
  EXPECT_TRUE(LoadConfig(WriteTempFile("commented_config.json", text)));
}

TEST(ConfigTest, RejectsAMissingFile) {
  EXPECT_FALSE(LoadConfig("/nonexistent/config.json"));
}

TEST(ConfigTest, RejectsInvalidJson) {
  EXPECT_FALSE(LoadConfig(WriteTempFile("invalid_config.json", "{ \"graphics\": ")));
}

class MissingSettingTest : public testing::TestWithParam<std::string> {};

TEST_P(MissingSettingTest, RejectsTheConfig) {
  nlohmann::json json = ValidConfig();
  const nlohmann::json::json_pointer setting(GetParam());
  json[setting.parent_pointer()].erase(setting.back());
  EXPECT_FALSE(Load(json));
}

INSTANTIATE_TEST_SUITE_P(EverySetting, MissingSettingTest, testing::Values(
  "/graphics/backend", "/window/width", "/window/height", "/window/fullscreen",
  "/projects/default", "/components"));

class InvalidValueTest : public testing::TestWithParam<std::pair<std::string, nlohmann::json>> {};

TEST_P(InvalidValueTest, RejectsTheConfig) {
  nlohmann::json json = ValidConfig();
  json[nlohmann::json::json_pointer(GetParam().first)] = GetParam().second;
  EXPECT_FALSE(Load(json));
}

INSTANTIATE_TEST_SUITE_P(Values, InvalidValueTest, testing::Values(
  std::make_pair("/window/width", nlohmann::json(0)),
  std::make_pair("/window/height", nlohmann::json(-1)),
  std::make_pair("/window/width", nlohmann::json("wide")),
  std::make_pair("/components/2", nlohmann::json({ { "type", "Teleporter" } })),
  std::make_pair("/components/2", nlohmann::json({ { "type", "NextProjectKey" }, { "key", "NoSuchKey" } })),
  std::make_pair("/components/2", nlohmann::json({ { "type", "NextProjectKey" } })),
  std::make_pair("/components/2", nlohmann::json({ { "type", "ScriptedRun" }, { "frames", 0 } })),
  std::make_pair("/components/2", nlohmann::json({ { "type", "ScriptedRun" } }))));
} /* namespace */
} /* namespace app */
} /* namespace particle */
} /* namespace gem */
