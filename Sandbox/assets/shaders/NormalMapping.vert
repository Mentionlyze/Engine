#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_BigTangent;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPosition;

out VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	vs_out.FragPos = vec3(u_Transform * vec4(a_Pos, 1.0));
	vs_out.TexCoords = a_TexCoords;

	mat3 normalMatrix = transpose(inverse(mat3(u_Transform)));
	vec3 T = normalize(normalMatrix * a_Tangent);
	vec3 B = normalize(normalMatrix * a_BigTangent);
	vec3 N = normalize(normalMatrix * a_Normal);
	
	mat3 TBN = transpose(mat3(T, B, N));
	vs_out.TangentLightPos = TBN * u_LightPos;
    vs_out.TangentViewPos  = TBN * u_ViewPosition;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
}