//A vertex shader dedicated to coloring 
//debug axes and testing the infrastructure
//for supporting multiple shaders

#version 430

layout (std140, binding = 0) uniform CameraInfo {
  mat4	ProjectionView; 
  vec3	eye;
};

layout(location = 0) in vec3 vertexPosition_modelspace;

out vec4 ex_Color;

void main(void) {
  vec4 vertex = vec4(vertexPosition_modelspace,1.0);
  gl_Position = ProjectionView*vertex;

  if (vertex.x >= 0.05)
    ex_Color = vec4(1.0,0.0,0.0,0.0);
  else if (vertex.y >= 0.05)
    ex_Color = vec4(0.0,1.0,0.0,0.0);
  else if (vertex.z >= 0.05)
    ex_Color = vec4(0.0,0.0,1.0,0.0);
  else
    ex_Color = vec4(1.0,1.0,1.0,0.0);    
}