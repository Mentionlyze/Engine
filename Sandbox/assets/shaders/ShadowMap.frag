#version 330 core

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

uniform sampler2D texture_diffuse;
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPosition;

uniform bool u_Blinn;
uniform bool u_Gamma;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
	// diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDir = normalize(u_ViewPosition - fragPos);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	float distance = length(lightPos - fragPos);
	float attenuation = 1.0 / (u_Gamma ? distance : distance * distance);

	diffuse *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
}

vec3 Phong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
	// diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDir = normalize(u_ViewPosition - fragPos);
	float spec = 0.0;
	vec3 reflectDir = reflect(-lightDir, normal);
	spec = pow(max(dot(normal, reflectDir), 0.0), 128.0);
	vec3 specular = spec * lightColor;

	float max_distance = 1.5;
	float distance = length(lightPos - fragPos);
	float attenuation = 1.0 / (u_Gamma ? distance : distance * distance);

	diffuse *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
}

void main()
{
	vec3 color = texture(texture_diffuse, fs_in.TexCoord).rgb;
	vec3 lighting = vec3(0.0);

	vec3 lightingFilter = u_Blinn 
			? BlinnPhong(normalize(fs_in.Normal), fs_in.FragPos, u_LightPos, u_LightColor)
			: Phong(normalize(fs_in.Normal), fs_in.FragPos, u_LightPos, u_LightColor);

	lighting += lightingFilter;
	color *= lighting;

	if (u_Gamma)
		color = pow(color, vec3(1.0 / 2.2));		

	FragColor = vec4(color, 1.0);
}