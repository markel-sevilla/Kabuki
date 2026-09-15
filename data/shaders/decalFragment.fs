#version 430 core

uniform sampler2D depth;
uniform sampler2D decalTex;
uniform sampler2D decalNormal;

uniform mat4 invView;
uniform mat4 invProj;
uniform mat4 invModel;
uniform mat4 viewToCube;

uniform float width;
uniform float height;

uniform bool decalArea;
uniform bool decalVolume;
uniform float angleThreshold;


layout (location = 0) out vec4 decalDiffuse; 
layout (location = 1) out vec4 decalNorm; 

void main()
{
	vec2 UV = vec2( gl_FragCoord.x / width, gl_FragCoord.y / height);

    //extract depth
    float depthValue = texture(depth, UV).r;
    
	// Convert depth to clip space
    float zClip = depthValue * 2.0 - 1.0;

    // Reconstruct view-space position
    vec4 viewPos = invProj * vec4(UV.x * 2.0 - 1.0, UV.y * 2.0 - 1.0, zClip, 1.0);
    viewPos /= viewPos.w;
    
    // Tangent and Bitangent approximation
    vec3 T = normalize(dFdx(viewPos.xyz));
    vec3 B = normalize(dFdy(viewPos.xyz));

    //normal approx
    vec3 N = normalize(cross(T, B));

    // Transform to decal model space
    vec4 modelPos = invModel * (invView * viewPos);

    //only render volume
    if(decalVolume)
    {
        decalNorm = vec4(1);
        decalDiffuse = vec4(1);
        return;
    }

    // Check if inside the decal volume
    if (abs(modelPos.x) > 0.5 || abs(modelPos.y) > 0.5 || abs(modelPos.z) > 0.5)
    {
        discard;
    }

    // Compute decal UV coordinates
    vec2 decalUV = modelPos.xy + vec2(0.5);

    //take normal from map
    vec3 normalSample = texture(decalNormal, decalUV).xyz * 2.0 - vec3(1.0);

    // move to view
    mat3 TBN = mat3(T, B, N);
    vec3 viewNormal = normalize(TBN * normalSample);

    // Sample the decal texture
    vec4 decalColor = texture(decalTex, decalUV);

    //only render area geometry affected
    if(decalArea)
    {
        decalNorm = vec4(1);
        decalDiffuse = vec4(1);
        return;
    }

    vec3 cubeNorm = normalize(vec3(viewToCube * vec4(N,0)));

    //discard based on angle 
    float angle = acos(dot(vec3(0,0,-1), cubeNorm));

    //alpha and angle threshold
    if(decalColor.w < 1 || angle > angleThreshold)
       discard;

    //out to gBuffer textures
    decalNorm = vec4(viewNormal , 0); 

    //pass a constant shininess to all the decals
    decalDiffuse = vec4(decalColor.xyz, 0);
    
}