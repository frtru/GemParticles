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
 *
*************************************************************************/
#ifndef CAMERA_HH
#define CAMERA_HH

#include <map>
#include <string>

// TODO: This class is now bound to opengl manipulations,
// might be interesting to rename so that there's a possibility
// to add a directx compatible camera
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Gem {
namespace Particle {
namespace Camera {

void Init();
void Terminate();

// TODO: See if pointers affect the return type of these functions
// and if return by value is correct
glm::mat4 GetViewMatrix();
glm::mat4 GetProjectionMatrix();

// TODO: Add functions to set View parameters : EYE, TARGET, AND UP VECTORS

void SetViewMatrix(const glm::mat4& a_ViewMatrix);

void LookAt(const glm::vec3& a_Eye,
  const glm::vec3& a_Target,
  const glm::vec3& a_Up);

void SetProjectionMatrix(const glm::mat4& a_ProjectionMatrix);

void SetPerspectiveProjection(float a_fFOV, float a_fWidth,
  float a_fHeight, float a_fNear, float a_fFar);

void SetOrthoProjection(float a_fLeft, float a_fRight,
  float a_fBottom, float a_fTop,
  float a_fNear, float a_fFar);

} /* namespace Camera*/
} /* namespace Particle */
} /* namespace Gem */
#endif /* end of include guard: CAMERA_HH */
