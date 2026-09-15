#version  430 core

layout(std430, binding = 2) buffer PencilNoiseBuffer{

    vec2 pencilNoise[]; // use sssbo for hatching lines
};

uniform sampler2DArray hatchingTexture;

//input textures
uniform sampler2D gNormal;
uniform sampler2D gCurvature;
uniform sampler2D distortedTexture;
uniform sampler2D gPosition;

uniform float attenuator;                   // comepnsation to normalize the world position
uniform vec2 windowSize;
uniform int maxDarkness;                    // maximum dark texture to use
uniform bool lit;
uniform bool blending;
uniform bool useCurv;                       
uniform float mu = 0.05;                    // darkening coefficient
uniform vec2 textureOffset = vec2(0, 0); // this offset is used for compensation for the world coordinates, to draw somehwere inside the region
uniform float paperRoughness;         // paper roughness in the drawing
uniform bool usePencilNoise;

struct Light
{
    vec3 pos;
    vec3 dir;
    float intensity;
};

uniform Light light;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform float maxTriangles;
uniform float maxCurvature;
layout (location = 0) out vec4 FragColor;


vec2 worldBasedUV(vec2 UVs)
{
    vec3 positionBasedUV =  ( inverse(viewMat) * texture( gPosition, UVs )).rgb;

    vec2 clampedUV = textureOffset + (vec2( positionBasedUV.x , positionBasedUV.y ) / attenuator) * 0.5 + 0.5;
   
    return clampedUV;
}

vec3 Blending3Way(int mappedBrightness, vec2 UVs)
{
    vec3 color = vec3(1);

    vec2 clampedUV = worldBasedUV(UVs);
    
    if(!blending)
    {
       return  texture(hatchingTexture, vec3(clampedUV, mappedBrightness) ).rgb;
    }

    //we take the neighboring contour curvature for this pixel
    vec2 curvatures[3] = { 2 * texture( gCurvature, UVs + vec2( 1, 0)/ windowSize).xy - 1, 
                           2 * texture( gCurvature, UVs + vec2(-1, 0)/ windowSize).xy - 1,
                           2 * texture( gCurvature, UVs + vec2( 0,-1)/ windowSize).xy - 1};
    
    bool neg = false; 
    
    for(int i = 0; i < 3; i++)
    {
        //float angle = atan(curvatures[i].y, curvatures[i].x);
        
        float angle = i * 3.1415f / 12.f;

        if(neg)
           angle = -angle;

        neg = !neg;

        mat2 rotation = mat2(cos(-angle), -sin(-angle), sin(-angle), cos(-angle));
        
        // rotate the texture to sample
        vec2 newUV = rotation * clampedUV;
       
        if(!useCurv)
           newUV = clampedUV;

        // darken the cross points between hatches and blend softer textures
        if(color.x == 1)
           color = texture(hatchingTexture, vec3(newUV, mappedBrightness / (i + 1)) ).rgb;
        else
           color -= (1.f - color.x) * mu;
       
    }

    return color;

}

void main()
{
    vec2 UVs = gl_FragCoord.xy / windowSize;
    
    //get normal
    vec3 normal =  (texture(gNormal, UVs) * 2 - vec4(1)).rgb;

    vec3 lightDir = normalize(vec3(viewMat * vec4(light.dir, 0)));

    // Compute the diffuse intensity using the dot product (0,1)
    float brightness = dot(normal, lightDir);

    // Apply the square root function to adjust brightness
    float adjustedBrightness = sqrt(brightness * light.intensity );
   
    //choose which textue we want to use
    int mappedBrightness = maxDarkness - int(floor(maxDarkness * adjustedBrightness));

    vec3 col = Blending3Way(mappedBrightness, UVs);

    //preserve white background
    if(texture(gNormal, UVs).rgb != vec3(0))
       FragColor = vec4(  col , 1.0  );
    else
    {
       FragColor = vec4(vec3(1), 1.0);
    }

    if(lit)
    {
         FragColor = vec4(adjustedBrightness);
         return;
    }

    vec3 color = texture(distortedTexture, UVs).rgb;

    //blend with distortedTexture with the darkening mu
    if( color != vec3(1))
       FragColor = vec4(color - mu, 0.0);

    vec2 uvs = worldBasedUV(UVs);
    int indices[2] = { int(floor(uvs.x * windowSize.x) ), int(floor(uvs.y * windowSize.y * windowSize.x)) };

    // same with the normal for paper effects
    if(texture(gNormal, UVs).rgb != vec3(0) && usePencilNoise)
    {
       FragColor = vec4( FragColor - paperRoughness * dot(vec2(1,0), vec2( vec4(vec3(normalize( pencilNoise[ indices[0] + indices[1] ] ), 0.0 ), 0.f))) ); 
    }
    
}