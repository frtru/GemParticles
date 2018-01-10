/*************************************************************************
 * Copyright (c) 2016 Fran�ois Trudel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
*************************************************************************/
#include "utils/skybox.hh"

#include <iostream>
#include <mutex>

#include <GL/glew.h>

// shader utilities
#include "utils/shader_factory.hh"
#include "utils/shader_module.hh"

// texture utilities
#include "utils/texture_factory.hh"

namespace gem {
namespace particle {
namespace skybox {
namespace {
const GLfloat SKYBOX_VERTICES[] = {
  // positions          
  -1.0f,  1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

  -1.0f,  1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f,  1.0f
};

std::once_flag init_flag;
std::once_flag terminate_flag;
GLuint cubemap_id;
GLuint shader_program_id;
GLuint skybox_VAO_id;
GLuint skybox_positions_VBO_id;

std::vector<std::string> texture_file_names;

const std::string CUBE_MAP_TEXTURE_RIGHT_FILE_PATH  = "textures/skybox2/posx.jpg";
const std::string CUBE_MAP_TEXTURE_LEFT_FILE_PATH   = "textures/skybox2/negx.jpg";
const std::string CUBE_MAP_TEXTURE_TOP_FILE_PATH    = "textures/skybox2/posy.jpg";
const std::string CUBE_MAP_TEXTURE_BOTTOM_FILE_PATH = "textures/skybox2/negy.jpg";
const std::string CUBE_MAP_TEXTURE_BACK_FILE_PATH   = "textures/skybox2/posz.jpg";
const std::string CUBE_MAP_TEXTURE_FRONT_FILE_PATH  = "textures/skybox2/negz.jpg";

void InitializeCube() {
  glGenVertexArrays(1, &skybox_VAO_id);
  std::cout << "skybox::InitializeCube -> Generated VAO ID = ";
  std::cout << skybox_VAO_id << std::endl;
  glBindVertexArray(skybox_VAO_id);
  std::cout << "skybox::InitializeCube -> Allocated array memory for ID = ";
  std::cout << skybox_VAO_id << std::endl;
  glGenBuffers(1, &skybox_positions_VBO_id);
  std::cout << "skybox::InitializeCube -> Generated VBO ID = ";
  std::cout << skybox_positions_VBO_id << std::endl;
  glBindBuffer(GL_ARRAY_BUFFER, skybox_positions_VBO_id);
  std::cout << "skybox::InitializeCube -> Allocated buffer memory for ID = ";
  std::cout << skybox_positions_VBO_id << std::endl;
  glBufferData(GL_ARRAY_BUFFER, sizeof(SKYBOX_VERTICES), SKYBOX_VERTICES, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  if (GL_ARB_vertex_attrib_binding) {
    glBindVertexBuffer(0, skybox_positions_VBO_id, 0, 3 * sizeof(GL_FLOAT));
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
  }
  else {
    glVertexAttribPointer(
      0, 3,
      GL_FLOAT, GL_FALSE,
      3 * sizeof(GL_FLOAT), nullptr);
  }
  glBindVertexArray(0);
}
}

void LoadSkyBox() {
  std::call_once(init_flag, [&]() {
    // If custom textures were not used
    if (texture_file_names.empty()) {
      texture_file_names.push_back(CUBE_MAP_TEXTURE_RIGHT_FILE_PATH);
      texture_file_names.push_back(CUBE_MAP_TEXTURE_LEFT_FILE_PATH);
      texture_file_names.push_back(CUBE_MAP_TEXTURE_BOTTOM_FILE_PATH);
      texture_file_names.push_back(CUBE_MAP_TEXTURE_TOP_FILE_PATH);
      texture_file_names.push_back(CUBE_MAP_TEXTURE_BACK_FILE_PATH);
      texture_file_names.push_back(CUBE_MAP_TEXTURE_FRONT_FILE_PATH);
    }
    cubemap_id = texture::factory::CreateCubeMap(texture_file_names);

    shader::factory::CompileShaderFile("skybox.vert", GL_VERTEX_SHADER);
    shader::factory::CompileShaderFile("skybox.frag", GL_FRAGMENT_SHADER);
    shader_program_id = shader::factory::CreateProgram();

    InitializeCube();
  });
}

void Destroy() {
  std::call_once(terminate_flag, [&]() {
    glDeleteBuffers(1, &skybox_positions_VBO_id);
  });
}

void SetTextures(std::vector<std::string>&& a_vTextures) {
  texture_file_names = std::move(a_vTextures);
}

void Render() {
  glDepthMask(GL_FALSE);
  shader::module::Use(shader_program_id);
  glBindVertexArray(skybox_VAO_id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_id);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthMask(GL_TRUE);
}
} /* namespace skybox */
} /* namespace particle */
} /* namespace gem */
