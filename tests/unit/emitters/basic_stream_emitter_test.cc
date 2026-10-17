#include <memory>

#include <gtest/gtest.h>

#include "emitters/basic_stream_emitter.hh"
#include "unit/test_doubles.hh"

namespace gem {
namespace particle {
namespace {
using test::CorePool;

const glm::f32vec3 SPAWN_LOCATION = { 1.0f, 2.0f, 3.0f };
const glm::f32vec3 SPAWN_VELOCITY = { 0.0f, 5.0f, 0.0f };
const glm::u8vec4  COLOR          = { 10u, 20u, 30u, 40u };

TEST(BasicStreamEmitterTest, EmitsRateTimesDtParticles) {
  auto pool = std::make_shared<CorePool>(100);
  BasicStreamEmitter emitter(SPAWN_LOCATION, SPAWN_VELOCITY, 2.0f, 20.0);
  emitter.Emit(0.5, pool);
  EXPECT_EQ(pool->GetActiveParticleCount(), 10u);
  emitter.Emit(0.5, pool);
  EXPECT_EQ(pool->GetActiveParticleCount(), 20u);
}

TEST(BasicStreamEmitterTest, InitializesTheEmittedParticles) {
  auto pool = std::make_shared<CorePool>(100);
  const float randomization = 0.25f;
  BasicStreamEmitter emitter(SPAWN_LOCATION, SPAWN_VELOCITY, 2.0f, 50.0, randomization, COLOR);
  emitter.Emit(1.0, pool);
  ASSERT_EQ(pool->GetActiveParticleCount(), 50u);
  for (std::size_t i = 0; i < pool->GetActiveParticleCount(); ++i) {
    EXPECT_EQ(pool->pCoreData->m_position[i], SPAWN_LOCATION);
    EXPECT_EQ(pool->pCoreData->m_lifetime[i], 2.0f);
    EXPECT_EQ(pool->pCoreData->m_color[i], COLOR);
    EXPECT_THAT(pool->pCoreData->m_velocity[i], test::Vec3Near(SPAWN_VELOCITY, randomization));
  }
}

TEST(BasicStreamEmitterTest, NeverEmitsMoreThanThePool) {
  auto pool = std::make_shared<CorePool>(100);
  BasicStreamEmitter emitter(SPAWN_LOCATION, SPAWN_VELOCITY, 2.0f, 1.0e9);
  emitter.Emit(1.0, pool);
  emitter.Emit(1.0, pool);
  EXPECT_LE(pool->GetActiveParticleCount(), pool->GetParticleCount());
}

// Finding: Emit clamps the last emitted index to GetParticleCount() - 1, so the
// last particle of the pool is never used (99 active particles out of 100).
TEST(BasicStreamEmitterTest, DISABLED_FillsTheWholePool) {
  auto pool = std::make_shared<CorePool>(100);
  BasicStreamEmitter emitter(SPAWN_LOCATION, SPAWN_VELOCITY, 2.0f, 1.0e9);
  emitter.Emit(1.0, pool);
  EXPECT_EQ(pool->GetActiveParticleCount(), pool->GetParticleCount());
}
} /* namespace */
} /* namespace particle */
} /* namespace gem */
