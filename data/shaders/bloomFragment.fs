#version 430 core
out vec4 FragColor;
  
layout(location = 0)in vec2 UV;

uniform sampler2D semiFinalImage;
  
uniform bool horizontal;
//arbitrary weights, should add a slider to change these in the future
uniform float weight[5] = float[] (0.23, 0.19, 0.12, 0.054, 0.016);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(semiFinalImage, 0); // gets size of single texel
    vec3 finalBlur = texture(semiFinalImage, UV).rgb * weight[0]; // current fragment's contribution
   
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            finalBlur += texture(semiFinalImage, UV + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            finalBlur += texture(semiFinalImage, UV - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            finalBlur += texture(semiFinalImage, UV + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            finalBlur += texture(semiFinalImage, UV - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    FragColor = vec4(finalBlur, 1.0);
}
