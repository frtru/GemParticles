//This geometry shader takes points as a 
//primitive input and return quads built with
//of triangle strips

#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (std140, binding = 0) uniform CameraInfo {
  mat4	ProjectionView;	// Projection * View
  vec3	eye;	// camera position
};

uniform float particle_size;

in	vec4 ex_Color[];
in  vec3 ex_Direction[]; // Only one direction per particle so index is always direction[0]
out vec2 TexCoord;
out	vec4 FragColor;

float angle_between_vector(vec3 v1, vec3 v2)
{
  return acos(
	dot(v1, v2)/ length(v1)*length(v2)
  );
}

vec3 project_vector_on_plane(vec3 v, vec3 normalized_plane_normal)
{
  return v - dot(v, normalized_plane_normal) * normalized_plane_normal;
}

vec4 quat_from_axis_angle(vec3 axis, float rad_angle)
{ 
  vec4 qr;
  float half_angle = 0.5*rad_angle;
  qr.x = axis.x * sin(half_angle);
  qr.y = axis.y * sin(half_angle);
  qr.z = axis.z * sin(half_angle);
  qr.w = cos(half_angle);
  return qr;
}

vec3 rotate_vertex_position(vec3 position, vec3 axis, float rad_angle)
{ 
  vec4 q = quat_from_axis_angle(axis, rad_angle);
  vec3 v = position.xyz;
  return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

void main(void) {
  // Built-in variable "gl_in" of the geometry shader
  // gives us the index in the primitive. Points only 
  // have one index, so gl_in[0] it is.

  vec3 wPos = gl_in[0].gl_Position.xyz;
  vec3 wRotatedPos = wPos;
  vec3 originalPos = wPos;
  vec3 wPosToCamera = normalize(eye - wPos);
  vec3 wUp = vec3(0.0, 1.0, 0.0); 
  vec3 wRight = cross(wPosToCamera, wUp);

  FragColor = ex_Color[0]; // Points only have one vertex

  // 0.1 - Project ex_Direction on 

  // 1- Compute angle between right and direction
  float wDirectionAngle = angle_between_vector(project_vector_on_plane(ex_Direction[0], wPosToCamera), wRight);

  // Lower left corner
  wPos -= (wRight * particle_size);
  wPos -= particle_size/2;
  wRotatedPos = rotate_vertex_position(wPos - originalPos, wPosToCamera, wDirectionAngle) + originalPos;
  gl_Position = ProjectionView * vec4(wRotatedPos, 1.0);
  TexCoord = vec2(0.0, 0.0);
  EmitVertex();

  // Upper left corner
  wPos.y += particle_size;
  wRotatedPos = rotate_vertex_position(wPos - originalPos, wPosToCamera, wDirectionAngle) + originalPos;
  gl_Position = ProjectionView * vec4(wRotatedPos, 1.0);
  TexCoord = vec2(0.0, 1.0);
  EmitVertex();

  // lower right corner
  wPos.y -= particle_size;
  wPos += wRight * 2*particle_size;
  wRotatedPos = rotate_vertex_position(wPos - originalPos, wPosToCamera, wDirectionAngle) + originalPos;
  gl_Position = ProjectionView * vec4(wRotatedPos, 1.0);
  TexCoord = vec2(1.0, 0.0);
  EmitVertex();

  // Upper right corner
  wPos.y += particle_size;
  wRotatedPos = rotate_vertex_position(wPos - originalPos, wPosToCamera, wDirectionAngle) + originalPos;
  gl_Position = ProjectionView * vec4(wRotatedPos, 1.0);
  TexCoord = vec2(1.0, 1.0);
  EmitVertex();

  EndPrimitive();
}