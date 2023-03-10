#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in mat4 a_InstanceMatrix;

out vec2 v_TexCoord;

uniform mat4 u_ViewProjection;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * a_InstanceMatrix * vec4(a_Pos, 1.0);
}