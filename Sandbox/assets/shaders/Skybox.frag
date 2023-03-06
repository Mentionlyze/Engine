#version 330 core
out vec4 color;

in vec3 v_TexCoord;

uniform samplerCube skybox;

void main()
{
	color = texture(skybox, v_TexCoord);
	//color = vec4(0.6, 0.2, 0.3, 1.0);
}