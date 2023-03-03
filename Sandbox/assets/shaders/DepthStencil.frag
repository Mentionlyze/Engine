#version 330 core

out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D texture1;

void main()
{
	color = texture(texture1, v_TexCoord);
	//color = vec4(0.7, 0.2, 0.3, 1.0);
}