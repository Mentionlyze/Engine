#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_TexCoords;

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec3 TexCoords;
} vs_out;

uniform mat4 u_ViewProjection;
uniform mat4 u_View;
uniform mat4 u_Transform;
uniform bool u_ReverseNormals;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	vs_out.FragPos = vec3(u_Transform * vec4(a_Pos, 1.0));
	if (u_ReverseNormals)
		vs_out.Normal = transpose(inverse(mat3(u_Transform))) * (1.0 - a_Normal);
	else 
		vs_out.Normal = transpose(inverse(mat3(u_Transform))) * a_Normal;
	vs_out.TexCoords = a_TexCoords;
}