#version 430

layout (std140, binding = 0) uniform CameraInfo {
  mat4	ProjectionView; 
  vec3	eye;
};

layout(location = 0) in vec3 vertexPosition_modelspace;

out vec3 TexCoords;

void main(void) {
  gl_Position = ProjectionView*vec4(vertexPosition_modelspace + eye,1.0);
  TexCoords = vertexPosition_modelspace;
  TexCoords.y *= -1.0f; // Since OpenGL's cubemaps start from top left instead of bottom left...
}