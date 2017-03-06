cmake_minimum_required(VERSION 3.6)
project(GemParticles)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/bin )
set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
if(WIN32)
  find_package(GLM REQUIRED)
  include_directories(${GLM_INCLUDE_DIRS})
endif()
if(UNIX AND NOT WIN32)
  find_package(glm REQUIRED)
  include_directories(${glm_INCLUDE_DIRS})
endif()
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

if(OpenGL_FOUND)
    MESSAGE("OpenGL found")
	include_directories(${OPENGL_INCLUDE_DIRS})
endif()
if(glm_FOUND OR GLM_FOUND)
	MESSAGE("GLM found")
endif()
if(GLEW_FOUND)
    MESSAGE("GLEW found")
	include_directories(${GLEW_INCLUDE_DIRS})
endif()
if(glfw3_FOUND)
    MESSAGE("GLFW3 found")
	include_directories(${glfw3_INCLUDE_DIRS})
endif()

set(SOURCE_FILES
        src/app.cc
        src/app.hh
        src/basic_stream_source.cc
        src/basic_stream_source.hh
        src/camera.cc
        src/camera.hh
        src/default_dynamic.cc
        src/default_dynamic.hh
        src/dynamic.hh
        src/event_handler.cc
        src/event_handler.hh
        src/global_acceleration.cc
        src/global_acceleration.hh
        src/graphic_context.cc
        src/graphic_context.hh
        src/main.cc
        src/opengl_context.cc
        src/opengl_context.hh
        src/opengl_renderer.cc
        src/opengl_renderer.hh
        src/particle.hh
        src/particle_pool.cc
        src/particle_pool.hh
        src/particle_system.cc
        src/particle_system.hh
        src/particle_system_interface.hh
        src/rain_source.cc
        src/rain_source.hh
        src/random_fountain_source.cc
        src/random_fountain_source.hh
        src/renderer.hh
        src/shader.cc
        src/shader.hh
        src/source.cc
        src/source.hh
        src/spherical_stream_source.cc
        src/spherical_stream_source.hh
        src/stub_renderer.cc
        src/stub_renderer.hh
        src/timer.hh)

add_executable(GemParticles ${SOURCE_FILES})

target_link_libraries(GemParticles ${OPENGL_LIBRARIES} ${GLU_LIBRARIES} ${GLEW_LIBRARIES} glfw)
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
