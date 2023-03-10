#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;

out vec3 v_Normal;
out vec3 v_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	v_Position = vec3(u_Transform * vec4(a_Pos, 1.0));
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
}
