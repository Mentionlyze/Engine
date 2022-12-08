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

	float constant;
	float linear;
	float quadratic;
};

vec3 CalcDiffuse(vec3 normal, vec3 lightDir, vec3 diffuse, vec3 diffuseTexture);
vec3 CalcSpecular(vec3 reflectDir, vec3 viewDir, vec3 specular,  float shininess, vec3 specularTexture);

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
	//vec3 lightDir = normalize(-u_Light.lightDir);
	vec3 diffuse = CalcDiffuse(normal, lightDir, u_Light.diffuse, vec3(texture(u_Material.diffuse, v_TexCoord)));

 // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    vec3 viewDir = normalize(v_ViewPosition - v_FragPos); 
    vec3 specular = CalcSpecular(reflectDir, viewDir, u_Light.specular, u_Material.shininess, vec3(texture(u_Material.specular, v_TexCoord)));

	float distance = length(u_Light.position - v_FragPos);
	float attenuation = 1.0 / (u_Light.constant + u_Light.linear * distance + u_Light.quadratic * (distance * distance));

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = specular + diffuse + ambient;

	color = vec4(result, 1.0);
}

vec3 CalcDiffuse(vec3 normal, vec3 lightDir, vec3 diffuse, vec3 diffuseTexture)
{
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	return diffuse * (diffuseStrength * diffuseTexture);
}

vec3 CalcSpecular(vec3 reflectDir, vec3 viewDir, vec3 specular, float shininess, vec3 specularTexture)
{
	float specularStrength = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
	return specular * (specularStrength * specularTexture);
}