#ifndef TEST_DOUBLES_HH
#define TEST_DOUBLES_HH

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include <gmock/gmock.h>
#include <glm/glm.hpp>

#include "core/particle_pool_core.hh"
#include "core/particle_system_interface.hh"
#include "utils/graphic_contexts/graphic_context.hh"

namespace gem {
namespace particle {
namespace test {
class MockGraphicContext : public GraphicContext {
public:
  // Input callbacks a project registered, so tests can send it events
  using GraphicContext::GetInputCallbacks;

  MOCK_METHOD(void, NewFrame, (), (override));
  MOCK_METHOD(void, Update, (), (override));
  MOCK_METHOD(bool, PollWindowClosedEvent, (), (override));
  MOCK_METHOD(void, Reshape, (int a_width, int a_height), (override));
  MOCK_METHOD(void, RequestScreenshot, (const std::string& a_sPath), (override));
  MOCK_METHOD(void, ResetRenderState, (), (override));
  MOCK_METHOD(std::size_t, GetWindowWidth, (), (const, override));
  MOCK_METHOD(std::size_t, GetWindowHeight, (), (const, override));
  MOCK_METHOD(void, InitImpl, (const WindowOptions& a_Options), (override));
  MOCK_METHOD(void, TerminateImpl, (), (override));
};

class MockParticleSystem : public IParticleSystem {
public:
  MockParticleSystem(std::string a_sName, std::size_t a_unProgramID, std::size_t a_unActiveParticles)
    : m_sName(std::move(a_sName)) {
    ON_CALL(*this, GetSystemName()).WillByDefault(testing::ReturnRef(m_sName));
    ON_CALL(*this, GetProgramID()).WillByDefault(testing::Return(a_unProgramID));
    ON_CALL(*this, GetActiveParticlesCount()).WillByDefault(testing::Return(a_unActiveParticles));
  }
  MOCK_METHOD(std::size_t, GetProgramID, (), (const, override));
  MOCK_METHOD(std::size_t, GetActiveParticlesCount, (), (const, override));
  MOCK_METHOD(const std::string&, GetSystemName, (), (const, override));
  MOCK_METHOD(void, Update, (double a_dt), (override));
  MOCK_METHOD(void, Render, (), (override));

private:
  std::string m_sName;
};

using CorePool = ParticlePool<CoreParticles>;

// Wakes the next particle of the pool with the given attributes
inline void WakeParticle(CorePool& a_rPool, float a_fLifetime,
  const glm::f32vec3& a_Position = glm::f32vec3(0.0f),
  const glm::f32vec3& a_Velocity = glm::f32vec3(0.0f),
  const glm::u8vec4& a_Color = glm::u8vec4(0u)) {
  const std::size_t id = a_rPool.GetActiveParticleCount();
  a_rPool.pCoreData->m_lifetime[id] = a_fLifetime;
  a_rPool.pCoreData->m_position[id] = a_Position;
  a_rPool.pCoreData->m_velocity[id] = a_Velocity;
  a_rPool.pCoreData->m_color[id]    = a_Color;
  a_rPool.Wake(id);
}

// Writes a file in a folder of the system's temp directory and returns its path
inline std::string WriteTempFile(const std::string& a_sName, const std::string& a_sContent) {
  const std::filesystem::path directory = std::filesystem::temp_directory_path() / "gem_unit_tests";
  std::filesystem::create_directories(directory);
  const std::filesystem::path path = directory / a_sName;
  std::ofstream(path) << a_sContent;
  return path.string();
}

MATCHER_P2(Vec3Near, a_Expected, a_fTolerance, "") {
  return glm::all(glm::lessThanEqual(glm::abs(arg - a_Expected), glm::f32vec3(a_fTolerance)));
}
} /* namespace test */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: TEST_DOUBLES_HH */
