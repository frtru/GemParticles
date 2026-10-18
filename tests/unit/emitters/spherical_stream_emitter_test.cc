// Random emitter: checked on invariants of its distribution, not exact values
#include <memory>

#include <gtest/gtest.h>

#include "emitters/spherical_stream_emitter.hh"
#include "unit/test_doubles.hh"

namespace gem {
namespace particle {
namespace {
using test::CorePool;

const float TOLERANCE = 1.0e-4f;

TEST(SphericalStreamEmitterTest, SpawnsParticlesOnTheSphere) {
  auto pool = std::make_shared<CorePool>(1000);
  const float radius = 3.0f;
  const glm::f32vec3 velocity = { 0.0f, 1.0f, 0.0f };
  SphericalStreamEmitter emitter(glm::f32vec3(0.0f), velocity, radius, 2.0f, 500.0);
  emitter.Emit(1.0, pool);
  ASSERT_EQ(pool->GetActiveParticleCount(), 500u);
  for (std::size_t i = 0; i < pool->GetActiveParticleCount(); ++i) {
    EXPECT_NEAR(glm::length(pool->pCoreData->m_position[i]), radius, TOLERANCE);
    EXPECT_EQ(pool->pCoreData->m_velocity[i], velocity);
    EXPECT_EQ(pool->pCoreData->m_lifetime[i], 2.0f);
  }
}

TEST(SphericalStreamEmitterTest, SphereIsCenteredOnTheSpawnLocation) {
  auto pool = std::make_shared<CorePool>(1000);
  const float radius = 3.0f;
  const glm::f32vec3 center = { 10.0f, 0.0f, 0.0f };
  SphericalStreamEmitter emitter(center, glm::f32vec3(0.0f), radius, 2.0f, 500.0);
  emitter.Emit(1.0, pool);
  for (std::size_t i = 0; i < pool->GetActiveParticleCount(); ++i) {
    ASSERT_NEAR(glm::length(pool->pCoreData->m_position[i] - center), radius, TOLERANCE);
  }
}
} /* namespace */
} /* namespace particle */
} /* namespace gem */
