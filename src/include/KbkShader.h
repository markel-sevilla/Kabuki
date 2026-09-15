/**
 * @file    KShader.h
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
    class KShader
    {
        public:
        // the program ID
        unsigned int ID;
        
        /**
        * @brief  It constructs the shader  
        * @return void 
        */
        KShader() = delete;

        /**
        * @brief  It constructs the shader  
        * @param[in] vPath: vertex path
        * @param[in] fPath: fragment path
        * @return void 
        */
        explicit KShader(const char* vertexPath, const char* fragmentPath);
        
        ~KShader();

        /**
        * @brief  Sets a program in use
        * @return void 
        */
        void Use();

        /**
        * @brief  Sets a uniform in a shader (overload)
        * @param[in] name: nmae of the variable
        * @param[in] uniform: the uniform to set
        * @return void 
        */
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


          private:
        /**
        * @brief  It loads the shader from a vertex and a fragment  
        * @param[in] vertexPath: vertex shader path
        * @param[in] fragmentPath: fragment shader path
        * @return void 
        */
        
        void LoadShader(const char* vertexPath, const char* fragmentPath);
    };
}
