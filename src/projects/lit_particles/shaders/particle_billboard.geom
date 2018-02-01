//This geometry shader takes points as a 
//primitive input and return quads built with
//of triangle strips

#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (std140, binding = 0) uniform CameraInfo {
  mat4	ProjectionView;	// Projection * View
  vec3	eye;			// camera position
  vec3  target;
  vec3  up_vector;
};

uniform float particle_size;

in	vec4 ex_Color[];
in  vec3 ex_Direction[]; // Only one direction per particle so index is always direction[0]
out vec2 TexCoord;
out	vec4 FragColor;

void main(void) {
  vec3 wPos = gl_in[0].gl_Position.xyz;
  vec3 wDirection = ex_Direction[0].xyz;
  vec3 wCameraPlaneNormal = normalize(eye - target);
  vec3 wUp = up_vector; 
  
  float dot = dot(wDirection, wCameraPlaneNormal);
  if (abs(dot) < 1.0f)
  {
    wUp = cross(wCameraPlaneNormal, wDirection);
  }
  
  wUp = normalize(wUp) * particle_size;
  wDirection *= particle_size;

  FragColor = ex_Color[0]; // Points only have one vertex
  // Lower left corner
  gl_Position = ProjectionView * vec4(wPos + wUp - wDirection, 1.0);
  TexCoord = vec2(0.0, 0.0);
  EmitVertex();

  // Upper left corner
  gl_Position = ProjectionView * vec4(wPos - wUp - wDirection, 1.0);
  TexCoord = vec2(0.0, 1.0);
  EmitVertex();

  // lower right corner
  gl_Position = ProjectionView * vec4(wPos + wUp + wDirection, 1.0);
  TexCoord = vec2(1.0, 0.0);
  EmitVertex();

  // Upper right corner
  gl_Position = ProjectionView * vec4(wPos - wUp + wDirection, 1.0);
  TexCoord = vec2(1.0, 1.0);
  EmitVertex();

  EndPrimitive();
}