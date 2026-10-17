cmake_minimum_required(VERSION 3.10.0)

project(Tests C CXX)

find_package(GTest REQUIRED)
include(GoogleTest)

# Unit tests: the simulation core and the app's units, without a window
file(GLOB_RECURSE UNIT_TEST_SOURCES "unit/*.cc" "unit/*.hh")
add_executable(GemParticlesUnitTests ${UNIT_TEST_SOURCES})
target_include_directories(GemParticlesUnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# Objects of an OBJECT library only reach the targets linking it directly
target_link_libraries(GemParticlesUnitTests App GraphicContexts GTest::gmock_main)
set_target_properties(GemParticlesUnitTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
gtest_discover_tests(GemParticlesUnitTests
    DISCOVERY_MODE PRE_TEST
    PROPERTIES LABELS unit)

# System tests: run the app itself from scripted configs and check what it produced
file(GLOB_RECURSE SYSTEM_TEST_SOURCES "system/*.cc" "system/*.hh")
add_executable(GemParticlesSystemTests ${SYSTEM_TEST_SOURCES})
target_include_directories(GemParticlesSystemTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GemParticlesSystemTests Projects GraphicContexts nlohmann_json::nlohmann_json GTest::gtest_main)
target_compile_definitions(GemParticlesSystemTests PRIVATE GEM_APP_PATH="$<TARGET_FILE:GemParticlesApp>")
set_target_properties(GemParticlesSystemTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_dependencies(GemParticlesSystemTests GemParticlesApp)
gtest_discover_tests(GemParticlesSystemTests
    DISCOVERY_MODE PRE_TEST
    PROPERTIES LABELS system TIMEOUT 120)
