/**
 * @file    Shader.cpp
 * @brief   Wrap up for the shader OpenGl class 
 *
 * @details  It wraps up a shader and makes it easier fpr the pipeline to use it
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
 
#include "Shader.h"
#include "glm/glm.hpp"
#include "glad/glad.h"

namespace Graphics
{
    Shader::~Shader()
    {
        glDeleteProgram(ID);
    }

    void Shader::LoadShader(const char* vertexPath, const char* fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // close file handlers
            vShaderFile.close();
            fShaderFile.close();

            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" + e.what());
        }


        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        //I compile the shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        // errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            LOPG_TRACE("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + infoLog + std::endl + vertexPath );
        };

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        // errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            LOG_TRACE("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + infoLog + std::endl + fragmentPath );
        };

        // Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        // errors
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            LOG_TRACE("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + infoLog + std::endl + fragmentPath );
        }

        // after getting program just delete
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // use my program
    void Shader::Use()
    {
        glUseProgram(ID);
    }
    
    void Shader::SetUniform(const std::string& name, const glm::vec3& vec) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
    }

    void Shader::SetUniform(const std::string& name, const glm::vec2& vec) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y);
    }

    void Shader::SetUniform(const std::string& name, const float& f) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), f);
    }

    void Shader::SetUniform(const std::string& name, const bool& b) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), b);
    }

    void Shader::SetUniform(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::SetUniform(const std::string& name, const size_t& s) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<GLint>(s));
    }

    void Shader::SetUniform(const std::string& name, const int& i) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<GLint>(i));
    }

    void Shader::SetUniform(const std::string& name, const unsigned& i) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<GLint>(i));
    }

    void Shader::SetUniform(const std::string& name, const double& i) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<GLint>(i));
    }

    void Shader::SetUniform(const std::string& name, const glm::vec3* i) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 256, &i[0][0]);
    }

   
}
