#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "main/app.hh"
#include "main/options.hh"
#include "unit/test_doubles.hh"

namespace gem {
namespace particle {
namespace app {
namespace {
class OptionsTest : public testing::Test {
protected:
  // The project argument is checked against the registered projects
  static void SetUpTestSuite() { LoadProjects(); }

  std::optional<int> Parse(std::vector<const char*> a_Arguments) {
    a_Arguments.insert(a_Arguments.begin(), "GemParticlesApp");
    return ParseOptions(static_cast<int>(a_Arguments.size()), a_Arguments.data(), options);
  }

  Options options;
};

TEST_F(OptionsTest, DefaultsToTheConfigsSettings) {
  EXPECT_FALSE(Parse({}));
  EXPECT_TRUE(options.project.empty());
  EXPECT_TRUE(options.configPath.empty());
}

TEST_F(OptionsTest, TakesARegisteredProject) {
  EXPECT_FALSE(Parse({ "rain" }));
  EXPECT_EQ(options.project, "rain");
}

TEST_F(OptionsTest, RejectsAnUnknownProject) {
  const std::optional<int> exitCode = Parse({ "volcano" });
  ASSERT_TRUE(exitCode);
  EXPECT_NE(*exitCode, 0);
}

TEST_F(OptionsTest, MakesTheConfigPathAbsolute) {
  const std::filesystem::path config = test::WriteTempFile("options_config.json", "{}");
  const std::filesystem::path previous = std::filesystem::current_path();
  std::filesystem::current_path(config.parent_path());
  const std::optional<int> exitCode = Parse({ "--config", "options_config.json" });
  std::filesystem::current_path(previous);
  EXPECT_FALSE(exitCode);
  EXPECT_EQ(options.configPath, config.string());
}

TEST_F(OptionsTest, RejectsAMissingConfigFile) {
  const std::optional<int> exitCode = Parse({ "--config", "/nonexistent/config.json" });
  ASSERT_TRUE(exitCode);
  EXPECT_NE(*exitCode, 0);
}

TEST_F(OptionsTest, HelpStopsTheAppWithoutError) {
  testing::internal::CaptureStdout();
  const std::optional<int> exitCode = Parse({ "--help" });
  const std::string help = testing::internal::GetCapturedStdout();
  ASSERT_TRUE(exitCode);
  EXPECT_EQ(*exitCode, 0);
  EXPECT_NE(help.find("--config"), std::string::npos);
}
} /* namespace */
} /* namespace app */
} /* namespace particle */
} /* namespace gem */
