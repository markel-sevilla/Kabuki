#version 430 core
out vec4 FragColor;
  
layout(location = 0)in vec2 UV;

uniform sampler2DArray hatchingTextureArray;
uniform bool horizontal;
uniform int layer;

//arbitrary weights, should add a slider to change these in the future
uniform float weight[5] = float[] (0.23, 0.19, 0.12, 0.054, 0.016);

void main()
{        

    vec2 tex_offset = 1.0 / vec2(1600, 900); // gets size of single texel
    vec3 finalBlur = texture(hatchingTextureArray, vec3(UV, layer)).rgb * weight[0]; // current fragment's contribution
   
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            finalBlur += texture(hatchingTextureArray, vec3(UV + vec2(tex_offset.x * i, 0.0), layer)).rgb * weight[i];
            finalBlur += texture(hatchingTextureArray, vec3(UV - vec2(tex_offset.x * i, 0.0), layer)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            finalBlur += texture(hatchingTextureArray, vec3(UV + vec2(0.0, tex_offset.y * i), layer)).rgb * weight[i];
            finalBlur += texture(hatchingTextureArray, vec3(UV - vec2(0.0, tex_offset.y * i), layer)).rgb * weight[i];
        }
    }

    FragColor = vec4(finalBlur, 1.0);
}
