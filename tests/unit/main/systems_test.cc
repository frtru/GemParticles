// The app systems, run on a world whose components come from config entries
#include <filesystem>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "imgui.h"
#include "imgui_internal.h"

#include "main/app_registry.hh"
#include "main/app_world.hh"
#include "unit/test_doubles.hh"

namespace gem {
namespace particle {
namespace app {
namespace {
using test::MockGraphicContext;
using testing::_;
using testing::NiceMock;

class SystemTest : public testing::Test {
protected:
  void AddComponents(const nlohmann::json& a_Entries) {
    for (const nlohmann::json& entry : a_Entries) {
      AddComponent(entry, world.components);
    }
  }
  void Update(const std::string& a_sSystem) { AppSystems().at(a_sSystem).update(world); }

  NiceMock<MockGraphicContext> context;
  AppWorld world{ ComponentStore(), context, "rain" };
};

TEST_F(SystemTest, ScriptedRunExitsAfterItsFrames) {
  AddComponents({ { { "type", "ScriptedRun" }, { "frames", 3 } } });
  EXPECT_CALL(context, RequestScreenshot(_)).Times(0);
  Update("ScriptedRun");
  Update("ScriptedRun");
  EXPECT_FALSE(world.exitRequested);
  Update("ScriptedRun");
  EXPECT_TRUE(world.exitRequested);
}

// Relative screenshot paths are the user's: the app later moves to its own folder
TEST_F(SystemTest, ScriptedRunSavesTheLastFrame) {
  AddComponents({ { { "type", "ScriptedRun" }, { "frames", 2 }, { "screenshot", "last.png" } } });
  const std::string expected = (std::filesystem::current_path() / "last.png").string();
  Update("ScriptedRun");
  testing::Mock::VerifyAndClearExpectations(&context);
  EXPECT_CALL(context, RequestScreenshot(expected)).Times(1);
  Update("ScriptedRun");
  EXPECT_TRUE(world.exitRequested);
}

TEST_F(SystemTest, SystemsDoNothingWithoutTheirComponent) {
  EXPECT_CALL(context, RequestScreenshot(_)).Times(0);
  for (int frame = 0; frame < 10; ++frame) {
    Update("ScriptedRun");
    Update("NextProjectKey");
  }
  EXPECT_FALSE(world.exitRequested);
  EXPECT_FALSE(world.nextProjectRequested);
}

// Systems reading input or drawing windows run inside a headless ImGui frame
class ImGuiSystemTest : public SystemTest {
protected:
  void SetUp() override {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.DisplaySize = ImVec2(800.0f, 600.0f);
    io.DeltaTime   = 1.0f / 60.0f;
    unsigned char* pixels = nullptr;
    int width = 0, height = 0;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
  }
  void TearDown() override {
    ImGui::DestroyContext();
  }

  // Runs the system in a frame where the key is pressed
  void UpdateWithKeyPressed(const std::string& a_sSystem, ImGuiKey a_Key, bool a_bImGuiWantsKeyboard = false) {
    ImGui::GetIO().AddKeyEvent(a_Key, true);
    ImGui::NewFrame();
    ImGui::GetIO().WantCaptureKeyboard = a_bImGuiWantsKeyboard;
    Update(a_sSystem);
    ImGui::EndFrame();
    ImGui::GetIO().AddKeyEvent(a_Key, false);
  }
};

TEST_F(ImGuiSystemTest, NextProjectKeyRequestsTheNextProject) {
  AddComponents({ { { "type", "NextProjectKey" }, { "key", "N" } } });
  UpdateWithKeyPressed("NextProjectKey", ImGuiKey_M);
  EXPECT_FALSE(world.nextProjectRequested);
  UpdateWithKeyPressed("NextProjectKey", ImGuiKey_N);
  EXPECT_TRUE(world.nextProjectRequested);
}

TEST_F(ImGuiSystemTest, NextProjectKeyLeavesTypingToImGui) {
  AddComponents({ { { "type", "NextProjectKey" }, { "key", "N" } } });
  UpdateWithKeyPressed("NextProjectKey", ImGuiKey_N, /*a_bImGuiWantsKeyboard=*/true);
  EXPECT_FALSE(world.nextProjectRequested);
}

TEST_F(ImGuiSystemTest, DebugOverlayDrawsItsWindowsOnlyWithItsComponent) {
  ImGui::NewFrame();
  Update("DebugOverlay");
  ImGui::EndFrame();
  EXPECT_EQ(ImGui::FindWindowByName("Stats"), nullptr);

  AddComponents({ { { "type", "DebugOverlay" } } });
  ImGui::NewFrame();
  Update("DebugOverlay");
  ImGui::EndFrame();
  EXPECT_NE(ImGui::FindWindowByName("Stats"), nullptr);
  EXPECT_NE(ImGui::FindWindowByName("Property editor"), nullptr);
  EXPECT_NE(ImGui::FindWindowByName("Debugging logs"), nullptr);
}
} /* namespace */
} /* namespace app */
} /* namespace particle */
} /* namespace gem */
