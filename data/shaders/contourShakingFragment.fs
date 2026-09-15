#version 430 core

uniform sampler2D edgeMap;           // Texture containing contour edges + position map
uniform sampler2D posMap;            //positions in cam space
uniform vec2 windowSize;                       
uniform vec3 shakingCoefficients;    // shaking coefficients for amplitude, frequency ...
uniform vec2 attenuator = vec2(100.f, 100.f);
uniform mat4 invView;
uniform bool complexContour = false;
uniform float segmentCount = 1;
uniform vec3 randomFactor; 
uniform float maxRand = 10;       // maximum offset allowed
out vec4 FragColor;

void main() 
{   
    vec2 UVs = gl_FragCoord.xy / windowSize;
    
    vec2 segmentUV = floor(UVs * segmentCount);

    vec3 coeff = shakingCoefficients * randomFactor;
    
    vec4 viewPos = vec4(texture(posMap, UVs).rgb, 1.0);
    
    //world position
    vec4 worldPos = invView * viewPos;
    
    //uniformlxy randomize each segment of the mapping
    float xOffset = (coeff.x) * 
                     cos( coeff.y * worldPos.x / attenuator.x + coeff.z ) * 
                     sin(  coeff.y * worldPos.z / attenuator.x + coeff.z);
    
    float yOffset = (coeff.x) * 
                     sin( coeff.y * worldPos.y / attenuator.x + coeff.z ) * 
                     cos(  coeff.y * worldPos.z / attenuator.x + coeff.z);
    
    if(!complexContour)
    {
        xOffset = (coeff.x) * cos( coeff.y * UVs.x + coeff.z);
        yOffset = (coeff.x) * sin( coeff.y * UVs.y + coeff.z);
    }
    
    //distorted UVs
    vec2 distortedUVs = vec2(xOffset , yOffset) / ( maxRand *  1.f / segmentCount * windowSize );
    
    FragColor = vec4(vec3(0.5 * distortedUVs + 0.5, 1.0 ), 1.0);
}
