/**
 * @file    Shader.h
 * @brief   Wrap up for the shader OpenGl class 
 *
 * @details  It wraps up a shader and makes it easier fpr the pipeline to use it
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */

#pragma once
#include "glm/glm.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Graphics
{
    class Shader
    {
        public:
        // the program ID
        unsigned int ID;
        
        // constructor reads and builds the shader
        Shader() {
            ID = 0;
        }
        
        ~Shader();
        
        /**
        * @brief  It loads the shader from a vertex and a fragment  
        * @param[in] vertexPath: vertex shader path
        * @param[in] fragmentPath: fragment shader path
        * @return void 
        */
        
        void LoadShader(const char* vertexPath, const char* fragmentPath);

        // use/activate the shader
        void Use();

        void SetUniform(const std::string& name, const glm::vec3& vec) const;
        void SetUniform(const std::string& name, const glm::vec2& vec) const;
        void SetUniform(const std::string& name, const float& f) const;
        void SetUniform(const std::string& name, const size_t& s) const;
        void SetUniform(const std::string& name, const bool& b) const;
        void SetUniform(const std::string& name, const glm::mat4& mat) const;
        void SetUniform(const std::string& name, const int& i) const;
        void SetUniform(const std::string& name, const unsigned& i) const;
        void SetUniform(const std::string& name, const double& i) const;
        void SetUniform(const std::string& name, const glm::vec3* i) const;
    };
}
