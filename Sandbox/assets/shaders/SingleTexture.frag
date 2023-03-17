#version 330 core

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

uniform sampler2D texture_diffuse;

void main()
{
	FragColor = texture(texture_diffuse, fs_in.TexCoord);
	//FragColor = vec4(0.7, 0.2, 0.3, 1.0);
}