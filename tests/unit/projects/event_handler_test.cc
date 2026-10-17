// Camera control of the projects' event handlers, driven through the input callbacks they register
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include "dynamics/particle_attractor.hh"
#include "projects/attractors/attractor_event_handler.hh"
#include "projects/attractors/proximity_color_updater.hh"
#include "projects/light/light_event_handler.hh"
#include "projects/lit_particles/lit_particles_event_handler.hh"
#include "projects/skybox/skybox_event_handler.hh"
#include "unit/test_doubles.hh"
#include "utils/basic_event_handler.hh"
#include "utils/camera.hh"
#include "utils/imgui/imgui_property_editor.h"

namespace gem {
namespace particle {
namespace {
using test::MockGraphicContext;
using testing::NiceMock;

// Every project starts with the camera on a diagonal, looking at the origin
const glm::vec3 EYE               = { 4.0f, 4.0f, 4.0f };
const glm::vec3 POINT_OF_INTEREST = { 0.0f, 0.0f, 0.0f };
const glm::vec3 UP                = { 0.0f, 1.0f, 0.0f };
// A 1 pixel drag turns the view by 0.1 degree
const float     ONE_PIXEL_DRAG_TOLERANCE = 0.5f;

struct Handler {
  std::string name;
  std::function<void(const std::shared_ptr<GraphicContext>&)> init;
  bool reaimsWithSpace;  // Space looks back at the origin
};
void PrintTo(const Handler& a_Handler, std::ostream* a_pStream) { *a_pStream << a_Handler.name; }

void InitAttractorHandler(const std::shared_ptr<GraphicContext>& a_pContext) {
  // The handler keeps both, and reads the updater's colors
  attractor_project::event_handler::Init(a_pContext,
    std::make_shared<ParticleAttractor>(POINT_OF_INTEREST, 1.0f),
    std::make_shared<attractor_project::ProximityColorUpdater>(POINT_OF_INTEREST,
      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
}

std::vector<Handler> Handlers() {
  return {
    { "basic", &event_handler::Init, false },
    { "attractor", &InitAttractorHandler, true },
    { "light", &light_project::event_handler::Init, true },
    { "lit_particles", &lit_particles_project::event_handler::Init, true },
    { "skybox", &skybox_project::event_handler::Init, true },
  };
}

// Without camera::Init, the camera registered no uniform block: LookAt is pure math, no GL needed
class EventHandlerTest : public testing::TestWithParam<Handler> {
protected:
  void SetUp() override {
    camera::LookAt(EYE, POINT_OF_INTEREST, UP);
    GetParam().init(context);
  }
  void TearDown() override {
    // Handlers keep their drag state between projects: end any drag for the next test
    RightButton(GLFW_RELEASE);
    ImGuiPropertyEditor::GetInstance().Clear();
  }

  const InputCallbacks& Callbacks() const { return context->GetInputCallbacks(); }
  void RightButton(int a_nAction) { Callbacks().mouseButton(GLFW_MOUSE_BUTTON_RIGHT, a_nAction, 0); }
  void Cursor(double a_dX, double a_dY) { Callbacks().cursorPosition(a_dX, a_dY); }
  void Drag(double a_dFromX, double a_dToX) {
    RightButton(GLFW_PRESS);
    Cursor(a_dFromX, 100.0);
    Cursor(a_dToX, 100.0);
    RightButton(GLFW_RELEASE);
  }

  // Angle between where the camera looks and the direction of a point, in degrees
  static float DegreesTo(const glm::vec3& a_Point) {
    const glm::vec3 view = glm::normalize(camera::GetTargetPosition() - camera::GetEyePosition());
    const glm::vec3 toPoint = glm::normalize(a_Point - camera::GetEyePosition());
    return glm::degrees(std::acos(glm::clamp(glm::dot(view, toPoint), -1.0f, 1.0f)));
  }

  std::shared_ptr<NiceMock<MockGraphicContext>> context = std::make_shared<NiceMock<MockGraphicContext>>();
};

TEST_P(EventHandlerTest, CursorWithoutRightButtonDoesNotMoveTheCamera) {
  Cursor(100.0, 100.0);
  Cursor(300.0, 200.0);
  EXPECT_EQ(camera::GetEyePosition(), EYE);
  EXPECT_EQ(camera::GetTargetPosition(), POINT_OF_INTEREST);
}

TEST_P(EventHandlerTest, FirstCursorEventOfADragOnlyRecordsThePosition) {
  RightButton(GLFW_PRESS);
  Cursor(100.0, 100.0);
  EXPECT_EQ(camera::GetTargetPosition(), POINT_OF_INTEREST);
}

TEST_P(EventHandlerTest, DragTurnsTheViewWithoutMovingTheEye) {
  RightButton(GLFW_PRESS);
  Cursor(100.0, 100.0);
  Cursor(150.0, 100.0);
  EXPECT_EQ(camera::GetEyePosition(), EYE);
  EXPECT_NE(camera::GetTargetPosition(), POINT_OF_INTEREST);
}

TEST_P(EventHandlerTest, ReleasingTheRightButtonEndsTheDrag) {
  Drag(100.0, 100.0);
  const glm::vec3 target = camera::GetTargetPosition();
  Cursor(300.0, 200.0);
  EXPECT_EQ(camera::GetTargetPosition(), target);
}

// Finding: the handlers start from yaw -90 and pitch 0 (looking down -Z) whatever the
// camera looks at, so the second cursor event of a drag turns the view towards -Z:
// ~55 degrees away from the point of interest for the projects' diagonal start.
TEST_P(EventHandlerTest, DISABLED_DragStartsFromTheCurrentView) {
  RightButton(GLFW_PRESS);
  Cursor(100.0, 100.0);
  Cursor(101.0, 100.0);
  EXPECT_LT(DegreesTo(POINT_OF_INTEREST), ONE_PIXEL_DRAG_TOLERANCE);
}

// Finding: Space looks back at the origin without updating yaw and pitch,
// so the next drag jumps back to the view of the previous drag.
TEST_P(EventHandlerTest, DISABLED_DragAfterReaimingStartsFromTheCurrentView) {
  if (!GetParam().reaimsWithSpace) {
    GTEST_SKIP() << GetParam().name << " has no re-aim key";
  }
  Drag(100.0, 400.0);
  Callbacks().key(GLFW_KEY_SPACE, 0, GLFW_PRESS, 0);
  ASSERT_LT(DegreesTo(POINT_OF_INTEREST), 0.1f);
  RightButton(GLFW_PRESS);
  Cursor(100.0, 100.0);
  Cursor(101.0, 100.0);
  EXPECT_LT(DegreesTo(POINT_OF_INTEREST), ONE_PIXEL_DRAG_TOLERANCE);
}

INSTANTIATE_TEST_SUITE_P(Projects, EventHandlerTest, testing::ValuesIn(Handlers()),
  [](const testing::TestParamInfo<Handler>& a_Info) { return a_Info.param.name; });
} /* namespace */
} /* namespace particle */
} /* namespace gem */
