#version 430 core

uniform sampler2DArray hatchingTex; 

uniform float sigma_s;  
uniform float sigma_r;  
 
uniform float width = 1600.f;  
uniform float height = 900.f;  

uniform int kernelSize;

layout (location = 0 ) in vec2 UV;
out vec4 FragColor;

void main() 
{
    float sum = 0.0;
    float weightSum = 0.0;

    //denominators for the formula
    float spatialDenominator = 1.0f / (2.0 * 3.1415f * sigma_s * sigma_s) ;
    float rangeDenominator = 1.0f / (2.0 * 3.1415f * sigma_r * sigma_r) ;
    for(int i = 0; i < 32; i ++)
    {
           //sample from the texturres
       float centerValue = texture(hatchingTex, vec3(UV, i)).r;
     
       // Apply bilateral filter using spatial and range terms
       for (int x = -kernelSize; x <= kernelSize; x++) 
       {
           for (int y = -kernelSize; y <= kernelSize; y++) 
           {
               vec2 offset = vec2( float(x) / width, float(y) / height);
     
               float sampleValue = texture(hatchingTex, vec3(UV + offset, i)).r;
     
               // Spatial weight
               float spatialWeight = spatialDenominator * exp( - ( x * x + y * y) / (2.0 *  sigma_s * sigma_s)) ;
     
               // Range weight 
               float rangeWeight = rangeDenominator * exp( -pow( abs(sampleValue - centerValue), 2) / (2.0 * sigma_r * sigma_r));
     
               float weight = spatialWeight * rangeWeight;
               sum += sampleValue * weight;
               weightSum += weight;
           }
       }
    }
    

    // Normalize
    FragColor = vec4(sum / weightSum, sum / weightSum, sum / weightSum, 1.f );  
}
