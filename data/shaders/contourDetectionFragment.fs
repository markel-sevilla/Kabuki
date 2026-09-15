#version 430 core

uniform sampler2D normalMap;  
uniform sampler2D depthMap;  
uniform float edgeThreshold; 
uniform float edgeThresholdDepth; 
uniform vec2 windowSize;
uniform float near;
uniform float far;
uniform float maxIntensity = 20.f;

out vec4 FragColor;

struct Light
{
    vec3 dir;
    float intensity;
};

uniform Light light;
uniform mat4 viewMat;

// Function to compute normal difference 
float normalDifference(vec3 n1, vec3 n2) 
{
    return length(n1 - n2);  
}

// Function to compute depth difference 
float depthDifference(float depth1, float depth2) 
{
    return abs(depth1 - depth2);  
}

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Convert depth to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

float getLightContribution(vec2 UVs)
{
    //get normal
    vec3 normal = ( (texture(normalMap, UVs) * 2 - 1)).rgb;

    vec3 lightDir = normalize(vec3( viewMat * vec4(light.dir, 0)));

    // Compute the diffuse intensity using the dot product (0,1)
    float brightness = dot(normal,lightDir) * 0.5 + 0.5;

    // Apply the square root function to adjust brightness
    return sqrt(brightness * light.intensity / maxIntensity ) ;
}

void main() 
{
    vec2 UVs = gl_FragCoord.xy / windowSize;
    vec2 texelSize = 1.0 / windowSize;  // Size of one pixel in UV coordinates

    float brightness = getLightContribution(UVs);

    // Sample normals from the normal map 
    vec3 normal00 = normalize(texture(normalMap, UVs + texelSize * vec2(-1, -1)).rgb * 2.0 - 1.0);
    vec3 normal01 = normalize(texture(normalMap, UVs + texelSize * vec2(-1,  0)).rgb * 2.0 - 1.0);
    vec3 normal02 = normalize(texture(normalMap, UVs + texelSize * vec2(-1,  1)).rgb * 2.0 - 1.0);
    
    vec3 normal10 = normalize(texture(normalMap, UVs + texelSize * vec2( 0, -1)).rgb * 2.0 - 1.0);
    vec3 normal11 = normalize(texture(normalMap, UVs + texelSize * vec2( 0,  0)).rgb * 2.0 - 1.0);  // Center pixel
    vec3 normal12 = normalize(texture(normalMap, UVs + texelSize * vec2( 0,  1)).rgb * 2.0 - 1.0);
    
    vec3 normal20 = normalize(texture(normalMap, UVs + texelSize * vec2( 1, -1)).rgb * 2.0 - 1.0);
    vec3 normal21 = normalize(texture(normalMap, UVs + texelSize * vec2( 1,  0)).rgb * 2.0 - 1.0);
    vec3 normal22 = normalize(texture(normalMap, UVs + texelSize * vec2( 1,  1)).rgb * 2.0 - 1.0);

     // Sample depth from the normal map 
    float depth00 = linearizeDepth((texture(depthMap, UVs + texelSize * vec2(-1, -1)).r ));
    float depth01 = linearizeDepth((texture(depthMap, UVs + texelSize * vec2(-1,  0)).r ));
    float depth02 = linearizeDepth((texture(depthMap, UVs + texelSize * vec2(-1,  1)).r ));
    
    float depth10 = linearizeDepth((texture(depthMap, UVs + texelSize * vec2( 0, -1)).r));
    float depth11 = linearizeDepth((texture(depthMap, UVs + texelSize * vec2( 0,  0)).r));  // Center pixel
    float depth12 = linearizeDepth((texture(depthMap, UVs + texelSize * vec2( 0,  1)).r));
    
    float depth20 = linearizeDepth((texture(depthMap, UVs + texelSize * vec2( 1, -1)).r));
    float depth21 = linearizeDepth((texture(depthMap, UVs + texelSize * vec2( 1,  0)).r));
    float depth22 = linearizeDepth((texture(depthMap, UVs + texelSize * vec2( 1,  1)).r));

    // Sobel filter for normal-based edge detection
    float sobelX = normalDifference(normal00, normal20) + 2.0 * normalDifference(normal01, normal21) + normalDifference(normal02, normal22);
    float sobelY = normalDifference(normal00, normal02) + 2.0 * normalDifference(normal10, normal12) + normalDifference(normal20, normal22);

    // Sobel filter for depth-based edge detection
    float sobelXdepth = depthDifference(depth00, depth20) + 2.0 *  depthDifference(depth01, depth21) + depthDifference(depth02, depth22);
    float sobelYdepth = depthDifference (depth00, depth02) + 2.0 * depthDifference(depth10, depth12) + depthDifference(depth20, depth22);

    // Compute edge strength
    float edgeStrengthNormal = sqrt(sobelX * sobelX + sobelY * sobelY);
    float edgeStrengthDepth = sqrt(sobelXdepth * sobelXdepth + sobelYdepth * sobelYdepth);
    
     //if there is no normal it is paper
    if(texture(normalMap, UVs).rgb == vec3(0) )
       brightness = 1.f;

    float colorNormal = smoothstep( edgeThreshold * 0.5, edgeThreshold,  edgeStrengthNormal / brightness);
    float colorDepth  = smoothstep( edgeThresholdDepth * 0.5, edgeThresholdDepth,  edgeStrengthDepth / brightness);

    FragColor =  vec4(vec3( colorNormal < colorDepth ? colorNormal : colorDepth) , 1.0);
}
