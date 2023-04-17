#version 330 core

out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D u_HDRBuffer;
uniform bool u_HDR;
uniform float u_Exposure;

void main()
{
	const float gamma = 2.0;
	vec3 hdrColor = texture(u_HDRBuffer, v_TexCoords).rgb;
	if (u_HDR)
	{
		// reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
		vec3 result = vec3(1.0) - exp(-hdrColor * u_Exposure);
		result = pow(result, vec3(1.0 / gamma));
		FragColor = vec4(result, 1.0);
	}
	else
	{
		vec3 result = pow(hdrColor, vec3(1.0 / gamma));
		FragColor = vec4(result, 1.0);
	}
}