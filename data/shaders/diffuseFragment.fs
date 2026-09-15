#version 430 core

layout (location = 0) in vec2 UV;
uniform sampler2D diffTex;

out vec4 FragColor;

void main()
{
	FragColor = texture(diffTex, UV).rgba;
}