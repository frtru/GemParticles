/*************************************************************************
 * Copyright (c) 2016 François Trudel
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
#include "camera.hh"

#include <mutex>

#include "glm/gtc/matrix_transform.hpp"

#include "shader_module.hh"

namespace camera {
namespace {
std::once_flag init_flag;
std::once_flag terminate_flag;
glm::mat4 projection_matrix;
glm::mat4 view_matrix;
glm::mat4 ProjectionView;
constexpr GLuint CameraInfoBindingPoint = 0;

// Perspective projection parameters
struct Perspective {
  GLfloat fov, width, height, near, far;
};
// Ortho projection parameters
struct Ortho {
  GLfloat left, right;
  GLfloat bottom, top;
  GLfloat near, far;
};

union ProjectionParameters {
  Perspective perspective;
  Ortho       ortho;
};
ProjectionParameters proj_params;

// View parameters
glm::vec3 eye_position;
glm::vec3 target_position;
glm::vec3 up_vector;

// Some helper functions
void UpdateProjectionView() {
  ProjectionView = camera::GetProjectionMatrix() * camera::GetViewMatrix();
  shader::module::SetUniformBlockValue(
    CameraInfoBindingPoint,
    0, 
    sizeof(ProjectionView),
    glm::value_ptr(ProjectionView)
  );
  shader::module::SetUniformBlockValue(
    CameraInfoBindingPoint,
    sizeof(ProjectionView),
    sizeof(eye_position),
    glm::value_ptr(eye_position)
  );
}

void UpdateViewMatrixAndProjectionView() {
  // UpdateProjectionView is called in LookAt
  LookAt(eye_position, target_position, up_vector);
}
}

void Init() {
  std::call_once(init_flag, [&]() {
    //glm::mat4() loads a 4x4 identity matrix
    projection_matrix = glm::mat4();
    view_matrix = glm::mat4();

    shader::module::RegisterGlobalUniformBlock(
      CameraInfoBindingPoint,
      sizeof(ProjectionView) + sizeof(eye_position));

    UpdateProjectionView();
  });
}

void Terminate() {
  std::call_once(terminate_flag, [&]() {
  });
}

const glm::vec3& GetEyePosition() {
  return eye_position;
}

const glm::vec3& GetTargetPosition() {
  return target_position;
}

const glm::vec3& GetUpVector() {
  return up_vector;
}

const glm::mat4& GetViewMatrix() {
  return view_matrix;
}

const glm::mat4& GetProjectionMatrix() {
  return projection_matrix;
}

// TODO: ...Maybe std::move this or expect && args?
// since it's not something clients may want to keep
void SetEyePosition(const glm::vec3& a_vEye) {
  eye_position = a_vEye;
  UpdateViewMatrixAndProjectionView();
}

void SetTargetPosition(const glm::vec3& a_vTarget) {
  target_position = a_vTarget;
  UpdateViewMatrixAndProjectionView();
}

void SetUpVector(const glm::vec3& a_vUp) {
  up_vector = a_vUp;
  UpdateViewMatrixAndProjectionView();
}

void SetViewMatrix(const glm::mat4& a_ViewMatrix) {
	view_matrix = a_ViewMatrix;
  UpdateProjectionView();
}

void LookAt(const glm::vec3& a_Eye, 
	const glm::vec3& a_Target,
	const glm::vec3& a_Up) {
  eye_position = a_Eye;
  target_position = a_Target;
  up_vector = a_Up;
  view_matrix = glm::lookAt(a_Eye, a_Target, a_Up);
  UpdateProjectionView();
}

void SetProjectionMatrix(const glm::mat4& a_ProjectionMatrix) {
	projection_matrix = a_ProjectionMatrix;
  UpdateProjectionView();
}

void SetPerspectiveProjection(float a_fFOV, float a_fWidth, 
	float a_fHeight, float a_fNear,	float a_fFar) {
  proj_params.perspective.fov     = a_fFOV;
  proj_params.perspective.width   = a_fWidth;
  proj_params.perspective.height  = a_fHeight;
  proj_params.perspective.near    = a_fNear;
  proj_params.perspective.far     = a_fFar;
  projection_matrix = glm::perspective(a_fFOV, a_fWidth/a_fHeight,
										 a_fNear, a_fFar);
  UpdateProjectionView();
}

void SetOrthoProjection(float a_fLeft	, float a_fRight, 
						float a_fBottom	, float a_fTop, 
						float a_fNear	, float a_fFar) {
  proj_params.ortho.left    = a_fLeft;
  proj_params.ortho.right   = a_fRight;
  proj_params.ortho.bottom  = a_fBottom;
  proj_params.ortho.top     = a_fTop;
  proj_params.ortho.near    = a_fNear;
  proj_params.ortho.far     = a_fFar;
  projection_matrix = glm::ortho(	a_fLeft, a_fRight,
		a_fBottom, a_fTop, a_fNear, a_fFar);
  UpdateProjectionView();
}
} /* namespace camera */