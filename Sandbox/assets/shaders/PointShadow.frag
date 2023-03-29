#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture_diffuse;
uniform samplerCube shadowMap;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPosition;

uniform float u_FarPlane;

float ShadowCalculation(vec3 fragPos)
{
     // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - u_LightPos;
    // Use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(shadowMap, fragToLight).r;
    // It is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= u_FarPlane;
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // Now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec3 color = texture(texture_diffuse, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(u_LightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(u_ViewPosition - fs_in.FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    //float shadow = ShadowCalculation(fs_in.FragPos);

    vec3 lighting = (ambient + diffuse + specular) * color;
    
    //FragColor = vec4(lighting, 1.0);
    //vec3 fragToLight = fs_in.FragPos - u_LightPos;
    
	FragColor = texture(shadowMap, fs_in.FragPos);
}
