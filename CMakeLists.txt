cmake_minimum_required (VERSION 3.0)
project (GemParticles)

#Version
set (GemParticles_VERSION_MAJOR 1)
set (GemParticles_VERSION_MINOR 0)

unset(TMP)

set (CMAKE_BINARY_DIR "bin")
set (CMAKE_SOURCE_DIR "src")

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
    message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
    message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

find_package(OpenGL REQUIRED)

add_subdirectory (external)
# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

include_directories(
    external/AntTweakBar-1.16/include/
    external/glfw-3.1.2/include/
    external/glm-0.9.7.1/
    external/glew-1.13.0/include/
    external/assimp-3.0.1270/include/
    src/
)

add_subdirectory(src)
