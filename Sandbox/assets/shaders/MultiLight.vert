#version 330 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TextCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_View;
uniform mat4 u_Transform;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	v_TexCoord = a_TextCoord;
	v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	v_FragPos = (u_Transform * vec4(a_Pos, 1.0)).xyz;
}
