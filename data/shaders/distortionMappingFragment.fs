#version 430 core

uniform sampler2D distortionMap;           // Texture containing distorted uvs
uniform sampler2D edgeMap;                 // Texture containing contour edges
uniform sampler2D previousMap;                 // Texture containing previous image

uniform bool prev;
uniform vec2 windowSize;             // Screen resolution
uniform float pencilTone;
out vec4 FragColor;                                        // Final pixel color




void main() 
{  
    vec2 UVs = gl_FragCoord.xy / windowSize;

    vec2 distortedUVs = UVs + (texture(distortionMap, UVs).xy * 2 - 1);

    vec4 col =  texture(previousMap, UVs);

    vec4 newCol =  vec4( 1.f -  pencilTone * texture(edgeMap, distortedUVs).xyz, 1.0);

    
    if( prev && col.r < 0.9 ) // if this pixel is already shaded we keep it that way 
    {
        if(newCol.r < 0.9)
        {
           FragColor = vec4( col.rgb  - vec3(0.1), 0) ; // pixel is painted? make it darker
        }
        else
        {
           FragColor =  vec4( col.rgb, 0); // pixel is white, paint it
        }
    }
    else
    {
        FragColor = newCol;
        
    }
    
}