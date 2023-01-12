#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_Porjection;
uniform mat4 u_View;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_Porjection * u_View * u_Transform * vec4(a_Pos, 1.0);
}
