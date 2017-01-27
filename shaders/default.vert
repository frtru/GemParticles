//A minimal implementation of a vertex shader 

#version 400

//uniform mat4x4 MVP;

layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 1) in vec4 vertexColor;

out vec4 ex_Color;

//in vec3 vp;

void main(void) {
  vec4 vertex = vec4(vertexPosition_modelspace,1.0);
  gl_Position = vertex;
  //ex_Color = vertexColor;
  
  if (vertex.x >= 0.05)
    ex_Color = vec4(1.0,0.0,0.0,0.0);
  else if (vertex.y >= 0.05)
    ex_Color = vec4(0.0,1.0,0.0,0.0);
  else if (vertex.z >= 0.05)
    ex_Color = vec4(0.0,0.0,1.0,0.0);
  else
    ex_Color = vec4(1.0,1.0,1.0,0.0);    
}