#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat3 normalMatrix;

void main()
{
    TexCoords = a_TexCoords;
    WorldPos = vec3(u_Transform * vec4(a_Pos, 1.0));
    Normal = normalMatrix * a_Normal;   

    gl_Position =  u_ViewProjection * vec4(WorldPos, 1.0);
}