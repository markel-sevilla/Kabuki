#version 430 core

layout (location = 0) in vec2 UVs;
layout (location = 0) out vec4 FragColor;

uniform sampler2D brightTexture;
uniform sampler2D pixelImage;
uniform bool bloomActive;

void main()
{
	if(bloomActive)
	{
	    FragColor = vec4(texture(brightTexture, UVs).rgb + texture(pixelImage, UVs).rgb, 1);
	}
	else
	{
		FragColor =  vec4(texture(pixelImage, UVs).rgb, 1);
	}
}