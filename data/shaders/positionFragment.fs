#version 430 core

layout (location = 0) in vec2 UV;
uniform sampler2D positionTex;

out vec4 FragColor;

void main()
{
	FragColor = texture(positionTex, UV).rgba;
}