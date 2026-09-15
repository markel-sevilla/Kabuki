#version 430 core

layout (location = 0) in vec2 UVs;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor; 

uniform float threshold;
uniform sampler2D semiFinalTexture;

void main()
{
	FragColor = vec4(texture(semiFinalTexture, UVs).rgb, 1);

    //random threshold
	float brightness = dot(FragColor.rgb, vec3(0.7, 0.7, 0.7));

    if(brightness > threshold)
    {
        BrightColor = vec4(FragColor.rgb, 1.0);
    }
    else
    {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}