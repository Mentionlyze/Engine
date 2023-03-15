#version 330 core

out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D texture_diffuse1;

void main()
{
	color = texture(texture_diffuse1, v_TexCoord);
	//color = vec4(0.3, 0.6, 0.2, 1.0);
}