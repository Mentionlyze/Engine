#version 330 core

out vec4 color;

in vec2 v_TexCoord;
uniform sampler2D u_Texture;

void main()
{
	color = vec4(1.0f, 0.5f, 0.31f, 1.0f);
}