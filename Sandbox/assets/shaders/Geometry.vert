#version 330 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

out vec3 v_FragPos;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_ViewPosition;

uniform mat4 u_ViewProjection;
uniform mat4 u_View;
uniform mat4 u_Transform;
uniform vec3 u_ViewPosition;

void main()
{
	v_FragPos = vec3(u_View * u_Transform * vec4(a_Pos, 1.0));
	v_TexCoord = a_TexCoord;
	v_Normal = mat3(transpose(inverse(u_View * u_Transform))) * a_Normal;
	v_ViewPosition = vec3(u_View * vec4(u_ViewPosition, 1.0));

	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
}