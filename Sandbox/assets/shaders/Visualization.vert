#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

out VS_OUT {
	vec3 normal;	
} vs_out;

uniform mat4 u_ViewProjection;
uniform mat4 u_View;
uniform mat4 u_Transform;

void main()
{
	mat3 normalMatrix = mat3(transpose(inverse(u_View * u_Transform)));
	vs_out.normal = vec3(vec4(normalMatrix * a_Normal, 0.0));
	gl_Position = u_View * u_Transform * vec4(a_Pos, 1.0);
}
