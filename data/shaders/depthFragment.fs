#version 430 core

layout (location = 0) in vec2 UV;

uniform sampler2D depthTex;
uniform float contrast;
uniform float near;
uniform float far;
out vec4 FragColor;

void main()
{
	float depth = (contrast * near) / (far + near - texture(depthTex, UV).r * (far - near));
	FragColor = vec4(depth, 0, depth, 1);
}