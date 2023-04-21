#version 330 core
out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform float weight[] = float[] (
	0.0024499299678342,
	0.0043538453346397,
	0.0073599963704157,
	0.0118349786570722,
	0.0181026699707781,
	0.0263392293891488,
	0.0364543006660986,
	0.0479932050577658,
	0.0601029809166942,
	0.0715974486241365,
	0.0811305381519717,
	0.0874493212267511,
	0.0896631113333857,
	0.0874493212267511,
	0.0811305381519717,
	0.0715974486241365,
	0.0601029809166942,
	0.0479932050577658,
	0.0364543006660986,
	0.0263392293891488,
	0.0181026699707781,
	0.0118349786570722,
	0.0073599963704157,
	0.0043538453346397,
	0.0024499299678342);

void main()
{             
     vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
     vec3 result = texture(image, v_TexCoords).rgb * weight[0];
     if(horizontal)
     {
         for(int i = 1; i < weight.length(); ++i)
         {
            result += texture(image, v_TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, v_TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1;	i < weight.length(); ++i)
         {
             result += texture(image, v_TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture(image, v_TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
         }
     }
     FragColor = vec4(result, 1.0);
     //FragColor = vec4(0.5 * (texture(image, v_TexCoords).rgb + vec3(1.0)), 1.0);
}