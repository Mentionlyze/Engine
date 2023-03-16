#version 330 core

out vec4 color;

in vec2 v_TexCoord;
uniform sampler2D diffuse;

void main()
{
	color = texture(diffuse, v_TexCoord);
}
