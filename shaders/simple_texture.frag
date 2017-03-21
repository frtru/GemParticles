//A minimal implementation of a fragment shader

#version 430

in  vec4 FragColor;
in	vec2 TexCoord;
out vec4 out_Color;

void main(void)
{
	out_Color = FragColor;
}