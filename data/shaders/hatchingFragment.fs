#version 430 core

layout(std430, binding = 0) buffer StrokeBuffer{

    float strokePositions[]; // use sssbo for hatching lines
};

layout(std430, binding = 1) buffer RandomBuffer{

    vec2 randomFactors[]; // use sssbo for hatching lines
};


uniform vec2 windowSize = vec2(1600, 900) ;
//for stroke perturbation
uniform float frequency = 0.5;
uniform float amplitude = 0.5;

//index of the texture in which we are
uniform float toneIndex;

uniform float mu_b = 0.1;  // Darkness increase factor (0.1 - 0.3)
uniform float mu_w = 0.5;  // White preservation factor (0.3 - 0.5)
const float cs = 1.f / 32.f; // color of the softest stroke
uniform int strokes;
const float PI = 3.14159265359;

layout (location = 0) in vec2 UV;
// out variables
out vec4 FragColor;

// Random function for perturbation
float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

// Function to draw a single stroke
vec2 drawStroke(vec2 uv, vec2 strokePos, vec2 randomFact) 
{
    //take the point in which te uv would be in the x
    vec2 point = mix(strokePos, vec2(1.0, strokePos.y), uv.x);

    vec2 dir = normalize( vec2(1.0, strokePos.y) - vec2(0.0, strokePos.y) );

    vec2 perp = vec2(-dir.y, dir.x); // perpendicular perturbation :)

    //sinusoidal func for soft curve
    float perturbation =  sin( randomFact.x * uv.x * frequency * 2.0 * PI) * amplitude * randomFact.y;
   
    return point + perp * perturbation;  // Creates a perturbed point
}


void main() 
{
    vec2 UVs = gl_FragCoord.xy / windowSize;

    // we check in which texture we are to detect the darkness
    // 32 textures 
    float ct = 1.0 - (toneIndex / 32.f);
    float ca = ct * (1.0 - cs);

    //preserve first texture as white
    if(toneIndex != 0)
    {
         // we will check all the strokes randomly generated 
         for (int i = 0; i < strokes; i++) 
         {
             // generate a stroke from one side to another
             vec2 shadedPoint = drawStroke(UVs, vec2(0.f, strokePositions[i]), randomFactors[i].xy);
         
             //if we don't have a stroke in this specific uv just continue
             if( int(floor(shadedPoint.y * windowSize.y)) ==  int(floor(UVs.y * windowSize.y)))
             {
                  // Compute stroke darkness modulation
             
                  if (ct > 0.9) 
                  {
                      ca *= mu_w; // Preserve white pixels
                  }
                 
                  ct -= mu_b * ca;
             }
         }
    }
   

    if(ct != 1.0 - (toneIndex / 32.f))
    {
         // Output grayscale pencil texture
         FragColor = vec4(vec3(ct), 1.0);
    }
    else
    {
         FragColor = vec4(1);
    }

}
