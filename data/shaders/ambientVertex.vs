#version 430 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aUV;

layout (location = 0) out vec2 UV;

void main() 
{ 
   gl_Position = aPos;
   UV = aUV; 
}