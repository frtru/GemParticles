//A minimal implementation of a vertex shader 

#version 430

layout (std140, binding = 0) uniform CameraInfo {
  mat4	ProjectionView; 
  vec3	eye;
};

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexColor;

out vec4 ex_Color;

void main(void) {
  gl_Position = vec4(vertexPosition_modelspace,1.0);
  ex_Color = vertexColor;
}