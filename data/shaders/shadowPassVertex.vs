#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 biTangent;
layout (location = 4) in vec2 aUV;
layout (location = 5) in vec4 aColor;

//matrices
uniform mat4 model;
uniform mat4 lightProj;
uniform mat4 lightView;

void main() 
{ 
   gl_Position = lightProj * ( lightView * ( model * aPos));
}