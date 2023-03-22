#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D texture_diffuse;
uniform sampler2D shadowMap;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPosition;
uniform vec3 u_LightColor;

uniform bool u_Blinn;
uniform bool u_Gamma;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

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
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(u_ViewPosition - fragPos);
    float spec = 0.0;
    vec3 reflectDir = reflect(-viewDir, normal);
    spec = pow(max(dot(normal, reflectDir), 0.0), 32.0);
    vec3 specular = spec * lightColor;

    return diffuse + specular;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 lightDir = normalize(u_LightPos - fs_in.FragPos);

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closetDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normalize(fs_in.Normal), lightDir)), 0.005);

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

    //float shadow = currentDepth - 0.005 > closetDepth ? 1.0 : 0.0;

    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{           
    vec3 color = texture(texture_diffuse, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.0);
    // ambient
    vec3 ambient = 0.3 * lightColor;

    vec3 lightingFilter = u_Blinn
         ? BlinnPhong(normal, fs_in.FragPos, u_LightPos, u_LightColor)
         : Phong(normal, fs_in.FragPos, u_LightPos, u_LightColor);

    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * lightingFilter) * color;    

    if (u_Gamma)
        lighting = pow(lighting, vec3(1.0 / 2.2));
    
    FragColor = vec4(lighting, 1.0);

    float depthValue = texture(shadowMap, fs_in.TexCoords).r;
    //FragColor = vec4(vec3(depthValue), 1.0);
}