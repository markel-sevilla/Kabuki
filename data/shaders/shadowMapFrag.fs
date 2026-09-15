#version 430 core

layout (location = 0) in vec2 UV;
uniform sampler2D depthMap;

layout ( location = 0 ) out vec4 outP; 

void main()
{
	outP = vec4(texture(depthMap, UV).r,texture(depthMap, UV).r,texture(depthMap, UV).r, 1);
}
