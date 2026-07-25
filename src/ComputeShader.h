#pragma once
#include "Shader.h"
#include "glad/glad.h"

namespace CS562
{
    class ComputeShader : public Shader
    {
    public:
        ComputeShader(const char* computePath)
        {
            LoadComputeShader(computePath);
        }
    
    private:
        void LoadComputeShader( const char* computePath);
        void CreateTexture();
    };
}
