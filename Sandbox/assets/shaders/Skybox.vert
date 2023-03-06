#version 330 core

layout (location = 0) in vec3 a_Pos;

out vec3 v_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	v_TexCoord = a_Pos;
	vec4 pos = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	gl_Position = pos.xyww;
}