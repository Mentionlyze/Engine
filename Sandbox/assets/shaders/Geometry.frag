#version 330 core

out vec4 color;

in vec3 v_FragPos;
in vec2 v_TexCoord;
in vec3 v_Normal;

uniform sampler2D u_Texture;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPosition;
uniform vec3 u_LightPos;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_LightColor;

	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPos - v_FragPos);
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diffuseStrength * u_LightColor;

 // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPosition - v_FragPos); 
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColor; 

	vec3 emission = vec3(texture(u_Texture, v_TexCoord));

	vec3 result = (specular + diffuse + ambient) * emission;

	color = vec4(result, 1.0);
}