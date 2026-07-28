/**
 * @file    ComputeShader.h
 * @brief   Wrap up for the compute shader OpenGl class 
 *
 * @details  It wraps up a shader and makes it easier fpr the pipeline to use it
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */

#pragma once
#include "Shader.h"
#include "glad/glad.h"

namespace CS562
{
    class ComputeShader : public Shader
    {
    public:

        /**
        * @brief  It constructs the wrap up foe the compute shader  
        * @param[in] computePath: the compute shader path
        * @return none 
        */
        ComputeShader(const char* computePath)
        {
            LoadComputeShader(computePath);
        }
    
    private:
         /**
        * @brief  It loads the shader from a vertex and a fragment  
        * @param[in] computePath: the compute shader path
        * @return void 
        */

        void LoadComputeShader( const char* computePath);
    };
}
