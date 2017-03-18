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
	src/core_opengl_renderer.cc
	src/core_opengl_renderer.hh
    src/cpu_particle_module.cc
    src/cpu_particle_module.hh
	src/dynamic.hh
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
	src/particle_core_data.hh
	src/particle_pool.hh
	src/particle_pool_core.hh
	src/particle_system.hh
	src/particle_system_interface.hh
	src/rain_emitter.cc
	src/rain_emitter.hh
	src/random_fountain_emitter.cc
	src/random_fountain_emitter.hh
	src/renderer.hh
	src/scene.cc
	src/scene.hh
	src/shader.cc
	src/shader.hh
	src/singleton.hh
	src/spherical_stream_emitter.cc
	src/spherical_stream_emitter.hh
	src/timer.hh
)

add_executable(GemParticles ${SOURCE_FILES})

if(WIN32 OR WINDOWS)
	target_link_libraries(GemParticles ${OPENGL_LIBRARIES} ${GLU_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW3_LIBRARIES} ${ANTWEAKBAR_LIBRARIES})
elseif(UNIX)
	target_link_libraries(GemParticles ${OPENGL_LIBRARIES} ${GLU_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW3_LIBRARIES} ${ANTWEAKBAR_LIBRARIES})
endif()
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

#if(WINDOWS OR WIN32)
#add_custom_command(TARGET GemParticles POST_BUILD
#	 COMMAND ${CMAKE_COMMAND} -E copy_if_different $ENV{GLFW_HOME}/built/lib/glfw3.dll $<CONFIG:RelWithDebInfo> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}			
#	 COMMAND ${CMAKE_COMMAND} -E copy_if_different $ENV{GLEW_HOME}/bin/Release/Win32/glew32.dll $<CONFIG:RelWithDebInfo> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}		
#)
#endif()