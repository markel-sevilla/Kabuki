#version 430 core
layout (location = 0) in vec4 camPos;
layout (location = 1) in vec3 myNormal;
layout (location = 2) in vec3 myTan;
layout (location = 3) in vec3 myBiTan;
layout (location = 4) in vec2 texCoord;
layout (location = 5) in vec4 vertexColor;
layout (location = 6) in float index;
layout (location = 7) in vec4 curv;
layout (location = 8) in vec3 avgNorm;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gDiffuse;
// layout (location = 3) out vec4 gTriangles;
// layout (location = 4) out vec4 gCurvature;


uniform sampler2D baseTexture;
uniform sampler2D normalMap;
uniform sampler2D specularTex;

uniform float maxCurvature;
uniform int maxTriangles;
uniform bool avgNormals;

void main()
{
	vec3 diffuse = texture(baseTexture, texCoord).rgb;
	vec3 specular = texture( specularTex, texCoord ).rgb;
	vec3 normal =  myNormal.xyz * 0.5 + 0.5; 
	vec3 avgNormal =  avgNorm.xyz * 0.5 + 0.5; 
	
	// Store data in the G-Buffer 
	gPosition = camPos;

	if(!avgNormals)
	   gNormal = vec4(normal, clamp(specular.g, 0, 1) * 20 + 5);
	else
	   gNormal = vec4(avgNormal, clamp(specular.g, 0, 1) * 20 + 5);

	gDiffuse = vec4(diffuse,  specular.b); 
	// gTriangles = vec4( float(index) / float(maxTriangles) );
	// gCurvature = curv * 0.5 + 0.5;
}