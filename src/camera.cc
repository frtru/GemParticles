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

#include "glm/gtc/matrix_transform.hpp"

namespace Gem {
namespace Particle {
namespace Camera {
namespace {
	// Todo: pointers or not when using global classes/namespaces?
	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	// TODO: Add
	// - position
	// - ???
}

// TODO: Find a way to prevent hardcoding values in here
void Init(GLenum a_eProjectionType) {
  LookAt( glm::vec3(0, 0, 1),   // Camera is at (0,0,1), in World Space
          glm::vec3(0, 0, 0),   // and looks at the origin
          glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)
  switch (a_eProjectionType) {
    case PROJ_PERSPECTIVE:
      SetPerspectiveProjection( glm::radians(45.0f), 
                                4.0f, 3.0f, 
                                0.1f, 100.0f);
      break;
    case PROJ_ORTHO:
      SetOrthoProjection( -10.0f, 10.0f, 
                          -10.0f, 10.0f, 
                          0.0f, 100.0f);
      break;
    default:
      //TODO: Log error for unrecognized projection type
      break;
  }
}

void Terminate() {

}

void SetViewMatrix(const glm::mat4& a_ViewMatrix) {
	view_matrix = a_ViewMatrix;
}

void LookAt(const glm::vec3& a_Eye, 
	const glm::vec3& a_Target,
	const glm::vec3& a_Up) {
	view_matrix = glm::lookAt(a_Eye, a_Target, a_Up);
}

void SetProjectionMatrix(const glm::mat4& a_ProjectionMatrix) {
	projection_matrix = a_ProjectionMatrix;
}

void SetPerspectiveProjection(float a_fFOV, float a_fWidth, 
	float a_fHeight, float a_fNear,	float a_fFar) {
	projection_matrix = glm::perspective(a_fFOV, a_fWidth/a_fHeight,
										 a_fNear, a_fFar);
}

void SetOrthoProjection(float a_fLeft	, float a_fRight, 
						float a_fBottom	, float a_fTop, 
						float a_fNear	, float a_fFar) {
	projection_matrix = glm::ortho(	a_fLeft, a_fRight,
		a_fBottom, a_fTop, a_fNear, a_fFar);
}

} /* namespace Camera */
} /* namespace Particle */
} /* namespace Gem */