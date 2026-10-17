// Every project runs a scripted number of frames and saves its last one
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <stb_image.h>

#include "projects/project_dictionary.hh"
#include "system/system_test_helpers.hh"

namespace gem {
namespace particle {
namespace system_test {
namespace {
std::vector<std::string> ProjectNames() {
  project_dict::Init();
  return project_dict::GetProjectNames();
}

class ScriptedRunTest : public testing::TestWithParam<std::string> {};

TEST_P(ScriptedRunTest, RendersTheProject) {
  if (!HasDisplay()) {
    GTEST_SKIP() << "No display to open the window on";
  }
  const std::filesystem::path directory = TestDirectory();
  nlohmann::json config = BaseConfig();
  config["components"].push_back({ { "type", "ScriptedRun" }, { "frames", 60 }, { "screenshot", "last_frame.png" } });

  // Relative to where the app is launched from
  const AppRun run = RunApp(directory, config, { GetParam() });
  ASSERT_EQ(run.exitCode, 0) << run.output;

  int width = 0, height = 0, channels = 0;
  stbi_set_flip_vertically_on_load(false);
  unsigned char* pixels = stbi_load((directory / "last_frame.png").c_str(), &width, &height, &channels, 3);
  ASSERT_NE(pixels, nullptr) << "No screenshot\n" << run.output;
  EXPECT_GT(width, 0);
  EXPECT_GT(height, 0);

  // Something was drawn over the clear color
  std::set<unsigned int> colors;
  for (int i = 0; i < width * height && colors.size() < 2; ++i) {
    colors.insert(pixels[3 * i] << 16 | pixels[3 * i + 1] << 8 | pixels[3 * i + 2]);
  }
  stbi_image_free(pixels);
  EXPECT_GT(colors.size(), 1u) << "The last frame has a single color";
}

INSTANTIATE_TEST_SUITE_P(Projects, ScriptedRunTest, testing::ValuesIn(ProjectNames()),
  [](const testing::TestParamInfo<std::string>& a_Info) { return a_Info.param; });
} /* namespace */
} /* namespace system_test */
} /* namespace particle */
} /* namespace gem */
