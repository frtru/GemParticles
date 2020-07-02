cmake_minimum_required(VERSION 3.10.0)

project(Rain C CXX)

file(GLOB SOURCE_FILES 
    "*.cc"
    "*.hh"
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Utils Core Dynamics Emitters Renderers ParticleSystems ${CONAN_LIBS})

add_custom_command(TARGET Rain POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/bin/shaders/${PROJECT_NAME}
)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
                                 ${Projects_INCLUDE_DIRS}
                                 ${ParticleSystems_INCLUDE_DIRS}
                                 ${Renderers_INCLUDE_DIRS}
                                 ${Emitters_INCLUDE_DIRS}
                                 ${Dynamics_INCLUDE_DIRS}
                                 ${Core_INCLUDE_DIRS}
                                 ${Utils_INCLUDE_DIRS}
                                 ${CONAN_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)