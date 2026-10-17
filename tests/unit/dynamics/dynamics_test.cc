#include <memory>

#include <gtest/gtest.h>

#include "dynamics/euler_particle_updater.hh"
#include "dynamics/gravity_acceleration.hh"
#include "dynamics/particle_attractor.hh"
#include "unit/test_doubles.hh"
#include "utils/imgui/imgui_property_editor.h"

namespace gem {
namespace particle {
namespace {
using test::CorePool;
using test::Vec3Near;
using test::WakeParticle;

const float TOLERANCE = 1.0e-5f;

TEST(EulerParticleUpdaterTest, AgesActiveParticles) {
  auto pool = std::make_shared<CorePool>(8);
  WakeParticle(*pool, 1.0f);
  WakeParticle(*pool, 2.0f);
  EulerParticleUpdater().Update(0.25, pool);
  EXPECT_FLOAT_EQ(pool->pCoreData->m_lifetime[0], 0.75f);
  EXPECT_FLOAT_EQ(pool->pCoreData->m_lifetime[1], 1.75f);
}

TEST(EulerParticleUpdaterTest, SleepsExpiredParticles) {
  auto pool = std::make_shared<CorePool>(8);
  WakeParticle(*pool, 1.0f);
  WakeParticle(*pool, 1.0f);
  WakeParticle(*pool, 0.1f);
  EulerParticleUpdater().Update(0.25, pool);
  EXPECT_EQ(pool->GetActiveParticleCount(), 2u);
}

TEST(EulerParticleUpdaterTest, MovesParticlesAlongTheirVelocity) {
  auto pool = std::make_shared<CorePool>(8);
  WakeParticle(*pool, 1.0f, { 1.0f, 2.0f, 3.0f }, { 4.0f, 0.0f, -2.0f });
  EulerParticleUpdater().Update(0.5, pool);
  EXPECT_THAT(pool->pCoreData->m_position[0], Vec3Near(glm::f32vec3(3.0f, 2.0f, 2.0f), TOLERANCE));
  EXPECT_EQ(pool->pCoreData->m_velocity[0], glm::f32vec3(4.0f, 0.0f, -2.0f));
}

// Finding: when particle i expires, Sleep(i) moves the last active particle into slot i
// and the loop goes on with i + 1, so that particle isn't aged during this update.
TEST(EulerParticleUpdaterTest, DISABLED_AgesTheParticleSwappedIntoAnExpiredSlot) {
  auto pool = std::make_shared<CorePool>(8);
  WakeParticle(*pool, 0.1f);
  WakeParticle(*pool, 1.0f);
  EulerParticleUpdater().Update(0.25, pool);
  ASSERT_EQ(pool->GetActiveParticleCount(), 1u);
  EXPECT_FLOAT_EQ(pool->pCoreData->m_lifetime[0], 0.75f);
}

TEST(GravityAccelerationTest, AcceleratesParticlesDownwards) {
  auto pool = std::make_shared<CorePool>(8);
  WakeParticle(*pool, 1.0f, glm::f32vec3(0.0f), { 1.0f, 0.0f, 0.0f });
  GravityAcceleration().Update(0.5, pool);
  EXPECT_THAT(pool->pCoreData->m_velocity[0], Vec3Near(glm::f32vec3(1.0f, -0.5f * 9.80665f, 0.0f), TOLERANCE));
  EXPECT_EQ(pool->pCoreData->m_position[0], glm::f32vec3(0.0f));
}

class ParticleAttractorTest : public testing::Test {
protected:
  // The attractor adds its properties to the editor; they point into it
  void TearDown() override { ImGuiPropertyEditor::GetInstance().Clear(); }
};

// The acceleration has the same magnitude at any distance: only its direction depends on the position
TEST_F(ParticleAttractorTest, AcceleratesTowardsTheAttractorThenMoves) {
  auto pool = std::make_shared<CorePool>(8);
  WakeParticle(*pool, 1.0f, glm::f32vec3(0.0f));
  WakeParticle(*pool, 1.0f, { 0.0f, 100.0f, 0.0f });
  ParticleAttractor attractor({ 10.0f, 0.0f, 0.0f }, 2.0f);
  attractor.Update(0.5, pool);

  EXPECT_THAT(pool->pCoreData->m_velocity[0], Vec3Near(glm::f32vec3(1.0f, 0.0f, 0.0f), TOLERANCE));
  EXPECT_THAT(pool->pCoreData->m_position[0], Vec3Near(glm::f32vec3(0.5f, 0.0f, 0.0f), TOLERANCE));
  EXPECT_NEAR(glm::length(pool->pCoreData->m_velocity[1]), 1.0f, TOLERANCE);
}
} /* namespace */
} /* namespace particle */
} /* namespace gem */
