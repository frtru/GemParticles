#include <gtest/gtest.h>

#include "unit/test_doubles.hh"

namespace gem {
namespace particle {
namespace {
using test::CorePool;
using test::WakeParticle;

TEST(ParticlePoolTest, StartsWithEveryParticleAsleep) {
  CorePool pool(8);
  EXPECT_EQ(pool.GetParticleCount(), 8u);
  EXPECT_EQ(pool.GetActiveParticleCount(), 0u);
}

TEST(ParticlePoolTest, WakeAndSleepCountActiveParticles) {
  CorePool pool(8);
  pool.Wake(0);
  pool.Wake(1);
  pool.Wake(2);
  EXPECT_EQ(pool.GetActiveParticleCount(), 3u);
  pool.Sleep(1);
  EXPECT_EQ(pool.GetActiveParticleCount(), 2u);
}

// Active particles stay at the front: the last active one takes the sleeping one's slot
TEST(ParticlePoolTest, SleepMovesTheLastActiveParticleIntoTheSlot) {
  CorePool pool(8);
  WakeParticle(pool, 1.0f, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1u, 1u, 1u, 1u });
  WakeParticle(pool, 2.0f, { 2.0f, 0.0f, 0.0f }, { 2.0f, 2.0f, 2.0f }, { 2u, 2u, 2u, 2u });
  WakeParticle(pool, 3.0f, { 3.0f, 0.0f, 0.0f }, { 3.0f, 3.0f, 3.0f }, { 3u, 3u, 3u, 3u });

  pool.Sleep(0);

  EXPECT_EQ(pool.pCoreData->m_lifetime[0], 3.0f);
  EXPECT_EQ(pool.pCoreData->m_position[0], glm::f32vec3(3.0f, 0.0f, 0.0f));
  EXPECT_EQ(pool.pCoreData->m_velocity[0], glm::f32vec3(3.0f));
  EXPECT_EQ(pool.pCoreData->m_color[0], glm::u8vec4(3u));
  EXPECT_EQ(pool.pCoreData->m_lifetime[1], 2.0f);
  // The sleeping particle is right after the active ones
  EXPECT_EQ(pool.pCoreData->m_lifetime[2], 1.0f);
}
} /* namespace */
} /* namespace particle */
} /* namespace gem */
