cmake_minimum_required(VERSION 3.10.0)

project(GemParticlesApp C CXX)

file(GLOB SOURCE_FILES 
    "*.cc"
    "*.hh"
    "systems/*.cc"
)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cc)

# The app without main(), so the tests can link it too. The systems register themselves
# while the program starts: as an OBJECT library, every registrar is linked.
add_library(App OBJECT ${SOURCE_FILES})
target_link_libraries(App PUBLIC
    GraphicContexts
    Utils 
    Core 
    Dynamics 
    Emitters 
    Renderers 
    ParticleSystems 
    Projects 
    CLI11::CLI11
    nlohmann_json::nlohmann_json
)

add_executable(${PROJECT_NAME} main.cc)

# Objects of an OBJECT library only reach the targets linking it directly
target_link_libraries(${PROJECT_NAME} App GraphicContexts)

set_target_properties(GemParticlesApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_property(TARGET GemParticlesApp PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_property(TARGET GemParticlesApp PROPERTY DEBUG_OUTPUT_NAME "GemParticlesApp_Debug")
set_property(TARGET GemParticlesApp PROPERTY RELEASE_OUTPUT_NAME "GemParticlesApp_Release")
set_property(TARGET GemParticlesApp PROPERTY RELWITHDEBINFO_OUTPUT_NAME "GemParticlesApp_RelWithDebInfo")
set_property(TARGET GemParticlesApp PROPERTY MINSIZEREL_OUTPUT_NAME "GemParticlesApp_MinSizeRel")

add_custom_command(TARGET GemParticlesApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_ROOT_PATH}/textures ${CMAKE_BINARY_DIR}/bin/textures
)
gem_copy_shaders(${PROJECT_NAME} ${PROJECT_ROOT_PATH}/shaders bootstrap)
# App settings next to the executable (copied again whenever the source changes)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.json ${CMAKE_BINARY_DIR}/bin/config.json COPYONLY)
             
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
                                 ${Projects_INCLUDE_DIRS}
                                 ${Renderers_INCLUDE_DIRS}
                                 ${Emitters_INCLUDE_DIRS}
                                 ${Dynamics_INCLUDE_DIRS}
                                 ${Core_INCLUDE_DIRS}
                                 ${Utils_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)