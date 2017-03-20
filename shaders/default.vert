//A minimal implementation of a vertex shader 

#version 430

layout (std140, binding = 0) uniform CameraInfo {
  mat4	MVP; 
  vec3	eye;
};

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexColor;

out vec4 ex_Color;

void main(void) {
  vec4 vertex = vec4(vertexPosition_modelspace,1.0);
  gl_Position = MVP*vertex;
  ex_Color = vertexColor;
}