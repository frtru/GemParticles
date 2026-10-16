cmake_minimum_required(VERSION 3.10.0)

project(Attractors C CXX)

file(GLOB SOURCE_FILES 
    "*.cc"
    "*.hh"
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# GCC/Clang only allow AVX intrinsics in code compiled for an AVX target
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set_source_files_properties(avx_particle_attractor.cc PROPERTIES COMPILE_OPTIONS "-mavx2;-mfma")
endif ()

target_link_libraries(${PROJECT_NAME} Utils Core Dynamics Emitters Renderers ParticleSystems ${CONAN_LIBS})

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