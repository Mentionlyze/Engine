#version 330 core

uniform vec3 u_LightColor;
out vec4 color;

void main()
{
	color = vec4(u_LightColor, 1.0f);
}