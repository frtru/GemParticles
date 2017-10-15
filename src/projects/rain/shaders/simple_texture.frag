//A minimal implementation of a fragment shader

#version 430

in  vec4 FragColor;
in	vec2 TexCoord;
out vec4 out_Color;

uniform sampler2D mytexture;

void main(void)
{
	out_Color = texture2D(mytexture, TexCoord) * FragColor;
}