//A minimal implementation of a fragment shader

#version 140

precision highp float; // needed only for version 1.30

in  vec3 ex_Color;
out vec4 out_Color;

void main(void)
{
	out_Color = vec4(ex_Color,1.0);
}