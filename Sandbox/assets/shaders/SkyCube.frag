#version 330 core

out vec4 color;

in vec3 v_Normal;
in vec3 v_Position;

uniform vec3 u_ViewPosition;
uniform samplerCube skybox;

void main()
{
	float ratio = 1.00 / 1.52;
	vec3 target = normalize(v_Position - u_ViewPosition);
	//vec3 reflectTarget = reflect(target, normalize(v_Normal));
	vec3 reflectTarget = refract(target, normalize(v_Normal), ratio);
	color = vec4(texture(skybox, reflectTarget).rgb, 1.0);
}
