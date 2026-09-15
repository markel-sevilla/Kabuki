#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 biTangent;
layout (location = 4) in vec2 aUV;
layout (location = 5) in vec4 aColor;
layout (location = 6) in float idx;
layout (location = 7) in vec2 curvature;
layout (location = 8) in vec3 avgNormal;

//matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


layout (location = 0) out vec4 camPos;
layout (location = 1) out vec3 myNormal;
layout (location = 2) out vec3 myTan;
layout (location = 3) out vec3 myBiTan;
layout (location = 4) out vec2 texCoord;
layout (location = 5) out vec4 vertexColor;
layout (location = 6) out float index;
layout (location = 7) out vec4 curv;
layout (location = 8) out vec3 avgNorm;

void main()
{
  
  vertexColor = aColor;
  texCoord = aUV;

  mat3 NormalMat = inverse(transpose(mat3( view * model))); 
  
  // Transform normal and tangent to camera space 
  myNormal = normalize(NormalMat * normal); 
  myTan = normalize( NormalMat * tangent); 
  
  // Compute the bitangent 
  myBiTan = normalize(NormalMat * biTangent); 
  
  // Transform position to camera space 
  camPos = (view * model * aPos); 
  texCoord = aUV;

  curv = proj * ( view * ( model * vec4(vec3(curvature, 0), 0)));
  
  avgNorm = normalize(NormalMat * avgNormal);
  
  index = idx;
  
  gl_Position = proj * ( view * ( model * aPos));
}