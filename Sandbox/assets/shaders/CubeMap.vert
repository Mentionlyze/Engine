#version 330 core

layout(location = 0) in vec3 a_Pos;

out vec3 WorldPos;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	WorldPos = a_Pos;
	gl_Position = u_Projection * u_View * vec4(WorldPos, 1.0);
}