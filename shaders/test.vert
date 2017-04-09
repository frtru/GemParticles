#version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

layout (std140, binding = 0) uniform CameraInfo {
  mat4	ProjectionView; 
  vec3	eye;
};

out vec2 tex_coord;

void main()
{
  vec4 vertex = vec4(position,1.0);
  gl_Position = ProjectionView*vertex;
  tex_coord = texcoord;
}