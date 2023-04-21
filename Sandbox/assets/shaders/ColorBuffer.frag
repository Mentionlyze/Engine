#version 330 core

out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D image;

void main()
{
	FragColor = texture(image, v_TexCoords);	
	//FragColor = vec4(0.7, 0.3, 0.4, 1.0);
}