cmake_minimum_required(VERSION 3.10.0)

project(Projects C CXX)

file(GLOB SOURCE_FILES 
    "*.cc"
    "*.hh"
)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(Projects_LIBS Attractors
                  Light
                  LitParticles
                  Rain
                  Skybox
    CACHE INTERNAL "Projects: Libs list" FORCE)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${Projects_LIBS})

add_subdirectory(attractors)
add_subdirectory(light)
add_subdirectory(lit_particles)
add_subdirectory(rain)
add_subdirectory(skybox)