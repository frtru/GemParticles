#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "core/particle_module.hh"
#include "unit/test_doubles.hh"

namespace gem {
namespace particle {
namespace {
using test::MockParticleSystem;
using testing::NiceMock;

class ParticleModuleTest : public testing::Test {
protected:
  void TearDown() override { particle_module::Terminate(); }

  // The module owns the system; the returned pointer stays valid until it is removed
  MockParticleSystem* AddSystem(const std::string& a_sName, std::size_t a_unProgramID,
    std::size_t a_unActiveParticles = 0) {
    auto system = std::make_unique<NiceMock<MockParticleSystem>>(a_sName, a_unProgramID, a_unActiveParticles);
    MockParticleSystem* pSystem = system.get();
    particle_module::AddSystem(std::move(system));
    return pSystem;
  }
};

TEST_F(ParticleModuleTest, FindsSystemsByName) {
  MockParticleSystem* rain = AddSystem("rain", 1);
  MockParticleSystem* snow = AddSystem("snow", 2);
  EXPECT_EQ(particle_module::GetSystemByName("rain"), rain);
  EXPECT_EQ(particle_module::GetSystemByName("snow"), snow);
  EXPECT_EQ(particle_module::GetSystemByName("hail"), nullptr);
}

TEST_F(ParticleModuleTest, RemovesSystemsByName) {
  AddSystem("rain", 1);
  AddSystem("snow", 2);
  particle_module::RemoveSystem("rain");
  EXPECT_EQ(particle_module::GetSystemByName("rain"), nullptr);
  EXPECT_NE(particle_module::GetSystemByName("snow"), nullptr);
}

TEST_F(ParticleModuleTest, CountsTheActiveParticlesOfEverySystem) {
  AddSystem("rain", 1, 100);
  AddSystem("snow", 2, 23);
  EXPECT_EQ(particle_module::GetActiveParticlesCount(), 123u);
}

// Systems are sorted by shader program, so consecutive ones share the GL program
TEST_F(ParticleModuleTest, UpdatesSystemsInProgramOrder) {
  MockParticleSystem* second = AddSystem("second", 7);
  MockParticleSystem* first  = AddSystem("first", 3);
  testing::InSequence order;
  EXPECT_CALL(*first, Update(0.5));
  EXPECT_CALL(*first, Render());
  EXPECT_CALL(*second, Update(0.5));
  EXPECT_CALL(*second, Render());
  particle_module::Update(0.5);
}

TEST_F(ParticleModuleTest, TerminateReleasesEverySystem) {
  AddSystem("rain", 1, 10);
  particle_module::Terminate();
  EXPECT_EQ(particle_module::GetSystemByName("rain"), nullptr);
  EXPECT_EQ(particle_module::GetActiveParticlesCount(), 0u);
}
} /* namespace */
} /* namespace particle */
} /* namespace gem */
