cmake_minimum_required(VERSION 3.10.0)

project(GemParticles)

# ----- You can tweak some common (for all subprojects) stuff here. -----

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Keep frame pointers in the profiling build so perf/valgrind get reliable call stacks
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO " -fno-omit-frame-pointer")
endif ()

# Remove 'lib' prefix for shared libraries on Windows
if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

# Find required packages
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(opengl_system REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(stb REQUIRED)
find_package(CLI11 REQUIRED)

# Project headers are included from src/; dependency headers come with their imported targets
include_directories(src)

set(PROJECT_ROOT_PATH ${PROJECT_SOURCE_DIR} 
    CACHE FILEPATH "PROJECT_ROOT_PATH: Project root path..." FORCE)                    

# Copies a shader folder to bin/shaders/<destination> on every build: the folders are tiny,
# and it lets shader-only edits reach bin/ without relinking anything.
function(gem_copy_shaders target source_dir destination)
  add_custom_target(${target}Shaders ALL
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_dir} ${CMAKE_BINARY_DIR}/bin/shaders/${destination})
  add_dependencies(${target} ${target}Shaders)
endfunction()

# When done tweaking common stuff, configure the components (subprojects).
# NOTE: The order matters! The most independent ones should go first.
add_subdirectory(src/utils)
add_subdirectory(src/core)
add_subdirectory(src/dynamics)
add_subdirectory(src/emitters)
add_subdirectory(src/renderers)
add_subdirectory(src/particle_systems)
add_subdirectory(src/projects)
add_subdirectory(src/main)