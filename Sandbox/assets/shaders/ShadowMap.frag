#version 330 core

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D texture_diffuse;
uniform sampler2D shadowMap;
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

	//diffuse *= attenuation;
	//specular *= attenuation;

	return diffuse + specular;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(u_LightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
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

	vec3 ambient = 0.3 * vec3(0.3);

	vec3 lighting = vec3(0.0);

	vec3 lightingFilter = u_Blinn 
		? BlinnPhong(normalize(fs_in.Normal), fs_in.FragPos, u_LightPos, u_LightColor)
		: Phong(normalize(fs_in.Normal), fs_in.FragPos, u_LightPos, u_LightColor);

	lighting += lightingFilter;

	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);  

	lighting *= (ambient + (1.0 - shadow));
	color *= lighting;

	if (u_Gamma)
		color = pow(color, vec3(1.0 / 2.2));		

	FragColor = vec4(color, 1.0);
}