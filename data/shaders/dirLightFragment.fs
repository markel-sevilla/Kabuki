#version 430 core

uniform sampler2D posTex; 
uniform sampler2D normalTex; 
uniform sampler2D diffuseTex; 

uniform sampler2D cascades[3];

uniform float depth[3];

uniform bool colorsBool;

uniform float bias;
uniform int pcf;
uniform float blendArea;

layout (location = 0) in vec2 UV;
layout (location = 0) out vec4 pixelSemiFinalImage; 

struct Light
{
	vec3 color;
    vec3 dir;
};

uniform Light light;
uniform vec3 viewPos;
uniform mat4 viewMat;

uniform mat4 lightMat[3];

//compute shadow
float Shadow(vec3 vertexPos, float bias, int x, int y)
{
    //distance from vertex to camera
    float dist = length( vertexPos - viewPos);

    float closestDepth = 0;
    vec3 projCoords;
    float shadow[2];

    if(dist < depth[0])
    {
         vec4 lightPos = lightMat[0] * vec4(vertexPos , 1);
        
         projCoords = lightPos.xyz / lightPos.w;
   
         projCoords = projCoords * 0.5 + 0.5; // from ndc to color

         vec2 texelSize = 1.0 / textureSize(cascades[0], 0);

        // get closest depth 
         closestDepth = texture(cascades[0], projCoords.xy + vec2(x, y) * texelSize).r; 
    }
    else if(dist < depth[1])
    {
        if(dist < depth[0] + blendArea)
        {
            //interpolate the weight
            float weight = (dist - depth[0]) / blendArea;
         
            //iterate through the two maps 
            for(int i = 0; i < 2; i++)
            {
                vec4 lightPos = lightMat[i] * vec4(vertexPos , 1);
        
                projCoords = lightPos.xyz / lightPos.w;
                
                projCoords = projCoords * 0.5 + 0.5; // from ndc to color
                
                vec2 texelSize = 1.0 / textureSize(cascades[i], 0);

                float weightedDepth = texture(cascades[i], projCoords.xy + vec2(x, y) * texelSize).r;
               
                shadow[i] = projCoords.z - bias > weightedDepth ? 1 : 0;
            }

            return shadow[0] * (1 - weight) + shadow[1] * weight;
        }
        else
        {
             vec4 lightPos = lightMat[1] * vec4(vertexPos , 1);
        
             projCoords = lightPos.xyz / lightPos.w;
             
             projCoords = projCoords * 0.5 + 0.5; // from ndc to color
             
             vec2 texelSize = 1.0 / textureSize(cascades[1], 0);
             
             // get closest depth 
             closestDepth = texture(cascades[1], projCoords.xy+ vec2(x, y) * texelSize).r; 
        }

    }
    else
    {
        if(dist < depth[1] + blendArea)
        {
            //interpolate the weight
            float weight = (dist - depth[1]) / blendArea;
            
            //iterate through the two maps 
            for(int j = 1; j < 3; j++)
            {
                vec4 lightPos = lightMat[j] * vec4(vertexPos , 1);
        
                projCoords = lightPos.xyz / lightPos.w;
                
                projCoords = projCoords * 0.5 + 0.5; // from ndc to color
                
                vec2 texelSize = 1.0 / textureSize(cascades[j], 0);

                // add to the closest depth the values for a smooth blend
                float weightedDepth = texture(cascades[j], projCoords.xy + vec2(x, y) * texelSize).r; 
                
                shadow[j - 1] = projCoords.z - bias > weightedDepth ? 1 : 0;
            }

            return shadow[0] * (1 - weight) + shadow[1] * weight;
        }
        else
        {
            vec4 lightPos = lightMat[2] * vec4(vertexPos , 1);

            projCoords = lightPos.xyz / lightPos.w;
   
            projCoords = projCoords * 0.5 + 0.5; // from ndc to color

            vec2 texelSize = 1.0 / textureSize(cascades[2], 0);
            
            // get closest depth 
            closestDepth = texture(cascades[2], projCoords.xy + vec2(x, y) * texelSize).r; 
        }
        
    }
  
    //avoid peter panning
    if(projCoords.z > 1.0)
        return 0.0;

   //return closestDepth;
   // is in shadow?
   return projCoords.z - bias > closestDepth ? 1.0 : 0;
}

vec3 phongLighting()
{
    // Read data from G-Buffer 
	vec4 diffTexValue = texture(diffuseTex, UV); 
	vec4 posTexValue = texture(posTex, UV);
	vec4 normalTexValue = (texture(normalTex, UV) + 1) * 0.5;
	
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

    vec3 L = normalize(-light.dir);
    vec3 V = normalize(viewPos - vertexPos);

    float att = 1 ;

    vec3 R = reflect(-L, N);
    
    // light color
    diffuse.x *= light.color.x;
    diffuse.y *= light.color.y;
    diffuse.z *= light.color.z;
    
    //diffuse
    diff =  max(dot(N,L), 0) * diffuse;
    
    float shadow = 0;


    //sample pcf 
    for(int x = -pcf; x <= pcf; ++x)
    {
        for(int y = -pcf; y <= pcf; ++y)
        {
            shadow += Shadow(vec3(inverse(viewMat) * vec4(vertexPos,1)) , bias, x, y);   
        }    
    }
    shadow /= (2 * pcf + 1) * (2 * pcf + 1);

    //return vec3(shadow);

     //distance from vertex to camera
    float dist = length( vec3(inverse(viewMat) * vec4(vertexPos,1)) - viewPos);

    //specular
    spec = vec3(pow(max(dot(R,V), 0.0), Ns) * specular);
   
   if(colorsBool)
   {
      if(dist < depth[0])
       {
          return diffuse * vec3(1, 0, 0);
       }
       
       if(dist < depth[1] && dist >= depth[0])
       {

          if(dist < depth[0] + blendArea)
             return diffuse * vec3(0.5, 0.5, 0);

           // get closest depth 
           return diffuse * vec3(0, 1, 0);
       }
       
       if(dist > depth[1])
       {
           if(dist < depth[1] + blendArea)
              return diffuse * vec3(0, 0.5, 0.5);

           // get closest depth 
           return diffuse * vec3(0, 0, 1);
       }
   }
    
   return (1 - shadow ) * att * (spec + diff);
}

void main() 
{ 
	pixelSemiFinalImage = vec4(phongLighting(), 1);
}