//A minimal implementation of a fragment shader

#version 430

in  vec4 ex_Color;
in	vec3 ex_Normal;
out vec4 out_Color;

uniform vec3 ambient_light_color;
uniform float ambient_light_intensity;

void main(void)
{
	// Basic ambient light
	vec3 ambient_light = ambient_light_color * ambient_light_intensity;
	out_Color = ex_Color * vec4(ambient_light,1.0); 
}