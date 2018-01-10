#version 430

in  vec3 TexCoords;
out vec4 out_Color;

uniform samplerCube skybox;

void main(void)
{
	out_Color = texture(skybox, TexCoords);
}