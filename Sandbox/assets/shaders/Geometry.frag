#version 330 core

struct Material 
{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 color;

in vec3 v_FragPos;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_ViewPosition;

uniform Material u_Material;
uniform Light u_Light;

void main()
{
	vec3 ambient = u_Material.ambient * u_Light.ambient;

	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(u_Light.position - v_FragPos);
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = u_Light.diffuse * (diffuseStrength * vec3(texture(u_Material.diffuse, v_TexCoord)));

 // specular
    vec3 viewDir = normalize(v_ViewPosition - v_FragPos); 
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * (spec * vec3(texture(u_Material.specular, v_TexCoord))); 

	vec3 result = specular + diffuse + ambient;

	color = vec4(result, 1.0);
}