//A vertex shader dedicated to coloring 
//debug axes and testing the infrastructure
//for supporting multiple shaders

#version 430

layout (std140, binding = 0) uniform CameraInfo {
  mat4	ProjectionView; 
  vec3	eye;
};

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;

out vec4 ex_Color;
out vec3 ex_Normal;

void main(void) {
  gl_Position = ProjectionView*vec4(vertexPosition_modelspace,1.0);
  ex_Color = vertexColor; 
  ex_Normal = vertexNormal;
}