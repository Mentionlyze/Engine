#version 330 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

struct Material
{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutoff;
	float outerCutoff;
};

uniform Material u_Material; 
uniform DirectLight u_DirectLight;
uniform SpotLight u_SpotLight;
uniform vec3 u_ViewPos;

vec3 CalcDirectLight(vec3 normal, vec3 viewDir, vec2 texCoord, Material material, DirectLight light);
vec3 CalcSpotLight(vec3 normal, vec3 lightDir, vec3 viewDir, float spotLightDistance, vec2 texCoord, Material material, SpotLight light);

out vec4 color;

void main()
{
	vec3 normal = normalize(v_Normal);
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);

	float spotLightDistance = length(u_ViewPos - v_FragPos);
	vec3 directResult = CalcDirectLight(normal, viewDir, v_TexCoord, u_Material, u_DirectLight);
	vec3 spotResult = CalcSpotLight(normal, viewDir, viewDir, spotLightDistance, v_TexCoord, u_Material, u_SpotLight);

	vec3 result = spotResult;
	color = vec4(result, 1.0);
}

vec3 CalcDirectLight(vec3 normal, vec3 viewDir,  vec2 texCoord, Material material, DirectLight light)
{
	// lightDir
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine result;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
	vec3 diffuse = light.diffuse * (diffuseStrength * vec3(texture(material.diffuse, texCoord)));
	vec3 specular = light.diffuse * (specularStrength * vec3(texture(material.specular, texCoord)));

	return ambient + diffuse + specular;
}

vec3 CalcSpotLight(vec3 normal, vec3 lightDir, vec3 viewDir, float spotLightDistance, vec2 texCoord, Material material, SpotLight light)
{
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// attenuation
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outerCutoff;
	float attenuation = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	// intensity
	float intensity = 1.0 / (light.constant + light.linear * spotLightDistance + light.quadratic * spotLightDistance * spotLightDistance);

	// combine result
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
	vec3 diffuse = light.diffuse * (diffuseStrength * vec3(texture(material.diffuse, texCoord)));
	vec3 specular = light.diffuse * (specularStrength * vec3(texture(material.specular, texCoord)));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}