cmake_minimum_required(VERSION 3.6)
project(GemParticles)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/bin )
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(AntTweakBar REQUIRED)

include_directories(${GLM_INCLUDE_DIRS})
if(OpenGL_FOUND)
    MESSAGE("OpenGL found")
	include_directories(${OPENGL_INCLUDE_DIRSS})
endif()
if(GLM_FOUND)
	MESSAGE("GLM found")
	include_directories(${GLM_INCLUDE_DIRS})
	MESSAGE(STATUS ${GLM_INCLUDE_DIRS})
endif()
if(GLEW_FOUND)
    MESSAGE("GLEW found")
	include_directories(${GLEW_INCLUDE_DIRS})
	MESSAGE(STATUS ${GLEW_INCLUDE_DIRS})
endif()
if(GLFW3_FOUND)
    MESSAGE("GLFW found")
	include_directories(${GLFW3_INCLUDE_DIRS})
	MESSAGE(STATUS ${GLFW3_INCLUDE_DIRS})
endif()
if(ANTTWEAKBAR_FOUND)
	MESSAGE("Antweakbar found")
	include_directories(${ANTTWEAKBAR_INCLUDE_DIRS})
	MESSAGE(STATUS ${ANTTWEAKBAR_INCLUDE_DIRS})
endif()

set(SOURCE_FILES
	src/app.cc
	src/app.hh
	src/basic_stream_emitter.cc
	src/basic_stream_emitter.hh
	src/camera.cc
	src/camera.hh
    src/cpu_particle_module.cc
    src/cpu_particle_module.hh
	src/dynamic.hh
	src/emitter.cc
	src/emitter.hh
	src/euler_particle_updater.cc
	src/euler_particle_updater.hh
	src/event_handler.cc
	src/event_handler.hh
	src/gravity_acceleration.cc
	src/gravity_acceleration.hh
	src/graphic_context.cc
	src/graphic_context.hh
	src/macro_definitions.hh
	src/main.cc
	src/opengl_context.cc
	src/opengl_context.hh
	src/opengl_renderer.cc
	src/opengl_renderer.hh
	src/particle_pool.cc
	src/particle_pool.hh
	src/particle_system.cc
	src/particle_system.hh
	src/rain_emitter.cc
	src/rain_emitter.hh
	src/random_fountain_emitter.cc
	src/random_fountain_emitter.hh
	src/renderer.hh
	src/shader.cc
	src/shader.hh
	src/simple_opengl_renderer.cc
	src/simple_opengl_renderer.hh
	src/spherical_stream_emitter.cc
	src/spherical_stream_emitter.hh
	src/timer.hh)

add_executable(GemParticles ${SOURCE_FILES})

target_link_libraries(GemParticles ${OPENGL_LIBRARIES} ${GLU_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW3_LIBRARIES} ${ANTWEAKBAR_LIBRARIES})
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
