//This geometry shader takes points as a 
//primitive input and return quads built with
//of triangle strips

#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (std140, binding = 0) uniform CameraInfo {
  mat4	MVP;	// In this application MVP = View/Projection
  vec3	eye;	// camera position
};

in	vec4 ex_Color;
out vec2 TexCoord;
out	vec4 FragColor;

// TODO: Pass the size as a parameter

void main(void) {
  // Built-in variable "gl_in" of the geometry shader
  // gives us the index in the primitive. Points only 
  // have one index, so gl_in[0] it is.

  vec3 wPos = gl_in[0].gl_Position.xyz;
  vec3 wPosToCamera = normalize(eye - wPos);
  // We could send up_vector as a uniform, but really 
  // it's always going to be oriented toward positive Y...
  vec3 wUp = vec3(0.0, 1.0, 0.0); 
  vec3 wRight = cross(wPosToCamera, wUp);

  FragColor = ex_Color;

  // Lower left corner
  wPos -= (wRight * 0.5);
  gl_Position = MVP * vec4(wPos, 1.0);
  TexCoord = vec2(0.0, 0.0);
  EmitVertex();

  // Upper left corner
  wPos.y += 1.0;
  gl_Position = MVP * vec4(wPos, 1.0);
  TexCoord = vec2(0.0, 1.0);
  EmitVertex();

  // lower right corner
  wPos.y -= 1.0;
  wPos += wRight;
  gl_Position = MVP * vec4(wPos, 1.0);
  TexCoord = vec2(1.0, 0.0);
  EmitVertex();

  // Upper right corner
  wPos.y += 1.0;
  gl_Position = MVP * vec4(wPos, 1.0);
  TexCoord = vec2(1.0, 1.0);
  EmitVertex();

  EndPrimitive();
}