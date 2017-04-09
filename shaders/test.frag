#version 430

out vec4 color;

in vec2 tex_coord;

// Texture samplers
uniform sampler2D mytexture;

void main()
{
	color = texture2D(mytexture, tex_coord);
}