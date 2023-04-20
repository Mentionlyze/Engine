#version 330 core

layout(binding = 0) uniform sampler2D texture_diffuse;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

struct Light {
	vec3 Position;
	vec3 Color;
};

uniform Light lights[4];
uniform vec3 u_ViewPosition;

void main()
{
	vec3 color = texture(texture_diffuse, fs_in.TexCoords).rgb;	
	vec3 normal = normalize(fs_in.Normal);

	vec3 ambient = 0.0 * color;
	vec3 lighting = vec3(0.0);
	vec3 viewDir = normalize(u_ViewPosition - fs_in.FragPos);
	for (int i = 0; i < 4; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = lights[i].Color * diff * color;      
        vec3 result = diffuse;        
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fs_in.FragPos - lights[i].Position);
        result *= 1.0 / (distance * distance);
        lighting += result;
    }

    vec3 result = ambient + lighting;
    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    FragColor = vec4(result, 1.0);
}	