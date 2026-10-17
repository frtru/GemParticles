#include <memory>

#include <gtest/gtest.h>

#include "unit/test_doubles.hh"
#include "utils/graphic_contexts/graphic_context.hh"
#include "utils/graphic_contexts/graphic_context_factory.hh"

namespace gem {
namespace particle {
namespace {
// The backends register themselves: this fails when the linker drops one
TEST(GraphicContextFactoryTest, CreatesTheOpenGLBackend) {
  EXPECT_NE(CreateGraphicContext("opengl"), nullptr);
}

TEST(GraphicContextFactoryTest, CreatesNothingForAnUnknownBackend) {
  testing::internal::CaptureStderr();
  EXPECT_EQ(CreateGraphicContext("vulkan"), nullptr);
  EXPECT_NE(testing::internal::GetCapturedStderr().find("opengl"), std::string::npos);
}

TEST(GraphicContextFactoryTest, CreatesRegisteredBackends) {
  ASSERT_TRUE(RegisterGraphicContext("test", []() -> std::unique_ptr<GraphicContext> {
    return std::make_unique<test::MockGraphicContext>();
  }));
  const std::unique_ptr<GraphicContext> context = CreateGraphicContext("test");
  EXPECT_NE(dynamic_cast<test::MockGraphicContext*>(context.get()), nullptr);
}

TEST(GraphicContextFactoryTest, KeepsTheFirstRegistrationOfABackend) {
  EXPECT_FALSE(RegisterGraphicContext("opengl", []() { return nullptr; }));
}
} /* namespace */
} /* namespace particle */
} /* namespace gem */
