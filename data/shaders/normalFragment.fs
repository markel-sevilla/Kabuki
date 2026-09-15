#version 430 core

layout (location = 0) in vec2 UV;
uniform sampler2D normalTex;

out vec4 FragColor;

void main()
{
	FragColor = texture(normalTex, UV).rgba;
}