cmake_minimum_required(VERSION 2.8)
project(AntTweakBar)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

find_package(glfw3 REQUIRED)

include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
if(glfw3_FOUND)
    MESSAGE("GLFW3 found")
	include_directories(${glfw3_INCLUDE_DIRS})
endif()

include_directories(include)

################### Build the library ###################
if (UNIX)
    set (platform UNIX)
elseif(WIN32)
	set(platform Windows)
else()
    message(SEND_ERROR "This platform (${CMAKE_SYSTEM_NAME}) is not handled yet! Consider a pull-request.")
endif()

set(anttweakbar_sources
    src/TwColors.cpp
    src/TwFonts.cpp
    src/TwOpenGLCore.cpp
    src/TwBar.cpp
    src/TwMgr.cpp
    src/LoadOGL.cpp
    src/LoadOGLCore.cpp
    src/TwEventGLFW.c
)

if(NOT DEFINED ATB_OPENGL_HEADER)
	set(ATB_OPENGL_HEADER GL/glcorearb.h)
endif()

if(NOT MSVC)
	string(REGEX REPLACE "-W[^ ]*" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing") # sigh
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_definitions("-DATB_OPENGL_HEADER=<${ATB_OPENGL_HEADER}>" -DTW_STATIC)

add_library(AntTweakBar STATIC ${anttweakbar_sources})