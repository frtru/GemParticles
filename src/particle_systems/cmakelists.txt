cmake_minimum_required(VERSION 3.10.0)

project(ParticleSystems C CXX)

file(GLOB SOURCE_FILES 
    "*.cc"
    "*.hh"
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Utils Core Dynamics Emitters Renderers ${CONAN_LIBS})

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
                                 ${Renderers_INCLUDE_DIRS}
                                 ${Emitters_INCLUDE_DIRS}
                                 ${Dynamics_INCLUDE_DIRS}
                                 ${Core_INCLUDE_DIRS}
                                 ${Utils_INCLUDE_DIRS}
                                 ${CONAN_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)