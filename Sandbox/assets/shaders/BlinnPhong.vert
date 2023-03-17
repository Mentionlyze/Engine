#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out VS_OUT {
	vec3 FragPos;	
	vec3 Normal;
	vec2 TexCoord;
} vs_out;

uniform mat4 u_ViewProjection;
uniform mat4 u_View;
uniform mat4 u_Transform;

void main()
{
	vs_out.FragPos = a_Pos;
	vs_out.Normal = a_Normal;
	vs_out.TexCoord = a_TexCoord;

	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
}