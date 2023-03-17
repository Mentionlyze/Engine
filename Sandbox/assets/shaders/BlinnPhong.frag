#version 330 core

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

uniform sampler2D diffuse;

uniform vec3 u_LightPositions[4];
uniform vec3 u_LightColors[4];
uniform vec3 u_LightPos;
uniform vec3 u_ViewPosition;

uniform bool u_Blinn;
uniform bool u_Gamma;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
	// diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDir = normalize(u_ViewPosition - fragPos);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	float max_distance = 1.5;
	float distance = length(lightPos - fragPos);
	float attenuation = 1.0 / (u_Gamma ? distance : distance * distance);

	diffuse *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
};

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
	spec = pow(max(dot(normal, reflectDir), 0.0), 8.0);
	vec3 specular = spec * lightColor;

	float max_distance = 1.5;
	float distance = length(lightPos - fragPos);
	float attenuation = 1.0 / (u_Gamma ? distance : distance * distance);

	diffuse *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
};


void main()
{
	vec3 color = texture(diffuse, fs_in.TexCoord).rgb;

	vec3 lighting = vec3(0.0);

	for(int i = 0; i < 4; i++) {
		vec3 lightingFilter = u_Blinn 
			? BlinnPhong(normalize(fs_in.Normal), fs_in.FragPos, u_LightPositions[i], u_LightColors[i])
			: Phong(normalize(fs_in.Normal), fs_in.FragPos, u_LightPositions[i], u_LightColors[i]);

		lighting += lightingFilter;
	}

	color *= lighting;

	if (u_Gamma)
		color = pow(color, vec3(1.0 / 2.2));		

	FragColor = vec4(color, 1.0);
	
	// ambient
//	vec3 ambient = 0.05 * color;
//
//	// diffuse
//	vec3 lightDir = normalize(u_LightPositions[0] - fs_in.FragPos);
//	vec3 normal = normalize(fs_in.Normal);
//	float diff= max(dot(lightDir, normal), 0.0);
//	vec3 diffuse = diff * color;
//
//	// specular
//	vec3 viewDir = normalize(u_ViewPosition - fs_in.FragPos);
//	vec3 reflectDir = reflect(-u_LightPositions[0], normal);
//	float spec = 0.0f;
//
//	if (u_Blinn)
//	{
//		vec3 halfwayDir = normalize(lightDir + viewDir);
//		spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
//	}
//	else 
//	{
//
//		vec3 reflectDir = reflect(-lightDir, normal);
//		spec = pow(max(dot(normal, reflectDir), 0.0), 8.0);
//	}
//
//	vec3 specular = vec3(0.3) * spec;
//
//	FragColor = vec4(ambient + diffuse + specular, 1.0);
}

