#version 430 core
uniform sampler2D posTex; 
uniform sampler2D normalTex; 
uniform sampler2D diffuseTex; 

layout (location = 0) out vec4 pixelSemiFinalImage; 

struct Light
{
    vec3 pos;
	vec3 color;
	float radius;
    vec3 dir;
};

uniform bool dirLight;
uniform Light light;
uniform vec3 viewPos;

uniform float width;
uniform float height;

vec3 phongLighting()
{
    vec2 UV = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);

    // Read data from G-Buffer 
	vec4 diffTexValue = texture(diffuseTex, UV); 
	vec4 posTexValue = texture(posTex, UV);
	vec4 normalTexValue = 2.0 * (texture(normalTex, UV)) - 1; 

	// Get values from data 
	vec3 diffuse = diffTexValue.rgb;
	float specular = diffTexValue.a;
	vec3 vertexPos = posTexValue.rgb;
	float Kd = posTexValue.a;
	vec3 N = normalTexValue.rgb;
	float Ns = normalTexValue.a;

    //ambient diffuse speculat
    vec3 diff;
    vec3 spec;

    vec3 L = normalize(light.pos - vertexPos);
    
    vec3 V = normalize(viewPos - vertexPos);

    //a
    float dist = length(light.pos - vertexPos);

    float att = 1 - min( dist / light.radius , 1);
    
    if(dirLight)
    {
        L = light.dir;
        att = 1;
    }

    vec3 R = reflect(-L, N);
    
    // light color
    diffuse.x *= light.color.x;
    diffuse.y *= light.color.y;
    diffuse.z *= light.color.z;
    
    //diffuse
    diff =  max(dot(N,L), 0) * diffuse;
    
    //specular
    spec = vec3(pow(max(dot(R,V), 0.0), Ns) * specular);
    
    return att * (spec + diff);
}

void main() 
{ 
	pixelSemiFinalImage = vec4(phongLighting(), 1);
}