cmake_minimum_required(VERSION 3.10.0)

project(Utils C CXX)

file(GLOB SOURCE_FILES
    "*.cc"
    "*.hh"
    "./imgui/*.h"
    "./graphic_contexts/*.hh"
)

include_directories(.)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    GLEW::GLEW
    glfw
    opengl::opengl
    glm::glm
    imgui::imgui
    stb::stb
)
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)

# Graphic context backends register themselves with the factory while the program starts.
# Nothing references them by name, so the linker would drop them from a static library:
# as an OBJECT library, the executable links every one of them.
file(GLOB GRAPHIC_CONTEXT_SOURCES "./graphic_contexts/*.cc")
# Dear ImGui's GLFW/OpenGL3 backends ship as sources in the conan package
set(IMGUI_BINDINGS_DIR ${imgui_INCLUDE_DIR}/../res/bindings)
add_library(GraphicContexts OBJECT ${GRAPHIC_CONTEXT_SOURCES}
    ${IMGUI_BINDINGS_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BINDINGS_DIR}/imgui_impl_opengl3.cpp
)
target_include_directories(GraphicContexts PRIVATE ${IMGUI_BINDINGS_DIR})
target_link_libraries(GraphicContexts PUBLIC ${PROJECT_NAME})

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
