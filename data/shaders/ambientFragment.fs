#version 430 core

layout ( location = 0) in vec2 UV;

uniform sampler2D baseTexture;
uniform sampler2D ambientMask;
uniform float ambient;
uniform bool occlussion;

layout (location = 0) out vec4 FragColor;

void main()
 {
	 float mask = texture(ambientMask, UV).r;

	 if(!occlussion)
	     mask = 1;
	 
     FragColor = vec4(ambient * mask * texture(baseTexture, UV).rgb, 1);
 }