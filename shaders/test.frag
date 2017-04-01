#version 430

out vec4 color;

in vec2 tex_coord;

// Texture samplers
uniform sampler2D mytexture;

void main()
{
	//color = vec4(tex_coord.x,tex_coord.y,0.0f,1.0f);
	color = texture2D(mytexture, tex_coord);
}