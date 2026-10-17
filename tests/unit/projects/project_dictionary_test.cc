#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "main/app.hh"
#include "projects/project_dictionary.hh"

namespace gem {
namespace particle {
namespace {
class ProjectDictionaryTest : public testing::Test {
protected:
  static void SetUpTestSuite() { app::LoadProjects(); }
};

TEST_F(ProjectDictionaryTest, ListsTheProjectsAlphabetically) {
  const std::vector<std::string> expected = { "attractor", "light", "lit_particles", "rain", "skybox" };
  EXPECT_EQ(project_dict::GetProjectNames(), expected);
}

TEST_F(ProjectDictionaryTest, LooksUpProjectsByName) {
  const Project* project = project_dict::LookUp("rain");
  ASSERT_NE(project, nullptr);
  EXPECT_TRUE(project->init);
  EXPECT_TRUE(project->update);
  EXPECT_TRUE(project->terminate);
  EXPECT_EQ(project_dict::LookUp("volcano"), nullptr);
}

TEST_F(ProjectDictionaryTest, NextProjectWrapsAround) {
  EXPECT_EQ(project_dict::NextProjectName("attractor"), "light");
  EXPECT_EQ(project_dict::NextProjectName("rain"), "skybox");
  EXPECT_EQ(project_dict::NextProjectName("skybox"), "attractor");
}
} /* namespace */
} /* namespace particle */
} /* namespace gem */
