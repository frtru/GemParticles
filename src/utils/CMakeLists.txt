cmake_minimum_required(VERSION 3.10.0)

project(Utils C CXX)

file(GLOB SOURCE_FILES
    "*.cc"
    "*.hh"
    "./imgui/*.h"
    "./graphic_contexts/*.hh"
    "./graphic_contexts/*.cc"
)

# Dear ImGui's GLFW/OpenGL3 backends ship as sources in the conan package
set(IMGUI_BINDINGS_DIR ${imgui_INCLUDE_DIR}/../res/bindings)

include_directories(.)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES}
    ${IMGUI_BINDINGS_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BINDINGS_DIR}/imgui_impl_opengl3.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_BINDINGS_DIR})

target_link_libraries(${PROJECT_NAME}
    GLEW::GLEW
    glfw
    opengl::opengl
    glm::glm
    imgui::imgui
    stb::stb
)
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
