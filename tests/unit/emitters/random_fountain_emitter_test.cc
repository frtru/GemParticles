// Random emitter: checked on invariants of its distribution, not exact values
#include <memory>

#include <gtest/gtest.h>

#include "emitters/random_fountain_emitter.hh"
#include "unit/test_doubles.hh"

namespace gem {
namespace particle {
namespace {
using test::CorePool;

const float TOLERANCE = 1.0e-4f;

TEST(RandomFountainEmitterTest, ShootsUpAtTheGivenSpeedInEveryDirection) {
  auto pool = std::make_shared<CorePool>(1000);
  const glm::f32vec3 location = { 1.0f, 2.0f, 3.0f };
  const float speed = 4.0f;
  RandomFountainEmitter emitter(location, 2.0f, 500.0, speed, { 1u, 2u, 3u, 4u });
  emitter.Emit(1.0, pool);
  ASSERT_EQ(pool->GetActiveParticleCount(), 500u);
  for (std::size_t i = 0; i < pool->GetActiveParticleCount(); ++i) {
    const glm::f32vec3 velocity = pool->pCoreData->m_velocity[i];
    EXPECT_NEAR(velocity.y, speed, TOLERANCE);
    EXPECT_NEAR(glm::length(glm::vec2(velocity.x, velocity.z)), speed, TOLERANCE);
    EXPECT_EQ(pool->pCoreData->m_position[i], location);
    EXPECT_EQ(pool->pCoreData->m_lifetime[i], 2.0f);
    EXPECT_EQ(pool->pCoreData->m_color[i], glm::u8vec4(1u, 2u, 3u, 4u));
  }
}
} /* namespace */
} /* namespace particle */
} /* namespace gem */
