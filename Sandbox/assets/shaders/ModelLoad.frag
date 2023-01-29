#version 330 core

out vec4 color;

in vec2 v_TexCoord;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
	vec4 diffuse = texture(texture_diffuse1, v_TexCoord);
	vec4 specular = texture(texture_specular1, v_TexCoord);

	vec4 result = diffuse;
	color = result;
	//color = vec4(0.7, 0.2, 0.3, 1.0);
}