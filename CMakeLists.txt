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
        src/*.cc
        src/*.hh)

add_executable(GemParticles ${SOURCE_FILES})

target_link_libraries(GemParticles ${OPENGL_LIBRARIES} ${GLU_LIBRARIES} ${GLEW_LIBRARIES} glfw)
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
