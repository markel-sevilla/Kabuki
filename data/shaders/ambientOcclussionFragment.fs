#version 430 core

uniform sampler2D depth;
uniform sampler2D normals;
uniform sampler2D positions;

uniform int AO_KERNEL_SIZE;
uniform float k;
uniform float sigma;
uniform float viewRadius;
uniform float bias;
uniform bool randomRot;
uniform vec2 samples[256];
uniform float zFar;
uniform float zNear;

uniform mat4 invProj;
uniform mat4 projMat;

uniform float width;
uniform float height;

out vec4 FragColor;

vec4 reconstructFromDepth(vec2 UVs)
{
    //extract depth
    float depthValue = texture(depth, UVs).r;
    
	// Convert depth to clip space
    float zClip = depthValue * 2.0 - 1.0;

    // Reconstruct view-space position
    vec4 viewPos = invProj * vec4((UVs.x) * 2.0 - 1.0, (UVs.y) * 2.0 - 1.0, zClip, 1.0);
    viewPos /= viewPos.w;

    return viewPos;
}

// Simple random function for rotating offsets
float rand(vec2 coords) {
    return fract(sin(dot(coords, vec2(12.9898, 78.233))) * 43758.5453);
}

// generate a random rotation matrix
mat2 randRot()
{
    // Generate random offset rotation
    float angle = rand(gl_FragCoord.xy) * (2 * 3.1415); 

    float c = cos(angle);
    float s = sin(angle);
    
    return mat2(c, -s, s, c);
}

void main()
{
    vec2 UVs = gl_FragCoord.xy / (vec2(width, height));
    //scale to screen
    vec4 viewPos = reconstructFromDepth(UVs);

    float NDCradius = (viewRadius * (projMat[0][0] / (-viewPos.z)));

    //visibility functaion
    float Ar = 0.0;
    float AO = 0.0;

    vec3 N = texture(normals, UVs).xyz * 2.0 - 1.0;

    //linearize depth
    float depthVal = texture(depth, UVs).r * 2.0 - 1;
    float linearDepth = (2.0 * zNear * zFar) / (zFar + zNear - depthVal * (zFar - zNear));
    linearDepth /= zFar;
       

    for(int i = 0; i < AO_KERNEL_SIZE; i++)
    {
        mat2 rotation = randRot();

        if(!randomRot)
        {
            rotation = mat2(1.f);
        }

       vec2 NDCuv = UVs * 2.0 - vec2(1.0);

       vec2 newUVs = (NDCuv + rotation * (NDCradius * samples[i]) + vec2(1.0)) * 0.5;
       
       //we want to rotate 
       vec4 sampleP = reconstructFromDepth(newUVs);

       //compute the v and the visibility wit the radius
       vec3 v =  sampleP.xyz - viewPos.xyz;
       
       if( length(v)  > viewRadius)
       {
           continue;
       }

       Ar += (max(0.0f, dot(v, N) - linearDepth * bias) / (dot(v, v) + 0.0001f) );
    }

    AO = pow(max(0.0f, 1.f - (2.0f * sigma / float(AO_KERNEL_SIZE)) * Ar), k);

    FragColor = vec4(AO, AO, AO, 1.0f);
}