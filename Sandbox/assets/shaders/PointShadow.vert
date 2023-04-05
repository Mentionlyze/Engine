#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 u_LightSpaceMatrix;

uniform bool u_ReverseNormals;

void main()
{
    vs_out.FragPos = vec3(u_Transform * vec4(aPos, 1.0));
    
    if (u_ReverseNormals)
        vs_out.Normal = transpose(inverse(mat3(u_Transform))) * (-1.0 * aNormal);
    else 
        vs_out.Normal = transpose(inverse(mat3(u_Transform))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0);
}