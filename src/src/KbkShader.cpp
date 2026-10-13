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
 
#include "KbkShader.h"
#include "glm/glm.hpp"
#include "glad/glad.h"
#include "Log.h"

namespace Graphics
{
    KShader::KShader(const char* vPath, const char* fPath)
    {
        LoadShader(vPath, fPath);
    }

    KShader::~KShader()
    {
        glDeleteProgram(ID);
    }

    bool KShader::LoadShader(const char* vertexPath, const char* fragmentPath)
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
            std::string msg = "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" ;
            msg += e.what();
            LOG_ERROR(msg.c_str());
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
            std::string msg = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog) + "\n" + std::string(vertexPath) ;

            LOG_TRACE(msg.c_str());
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

            std::string msg = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog) + "\n" + std::string(fragmentPath) ; 
            LOG_TRACE( msg.c_str());

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

            std::string msg = std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED\n") + std::string(infoLog) + "\n" + std::string(fragmentPath) ;
            LOG_TRACE(msg);
        }

        // after getting program just delete
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        LOG_TRACE("Shader bound to vertex " + std::string(vertexPath) + " and fragment " + std::string(fragmentPath) + " was succesfully created" );

        return true;
    }

    // use my program
    void KShader::Use()
    {
        glUseProgram(ID);
    }
    
    void KShader::SetUniform(const std::string& name, const glm::vec3& vec) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
    }

    void KShader::SetUniform(const std::string& name, const glm::vec2& vec) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y);
    }

    void KShader::SetUniform(const std::string& name, const float& f) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), f);
    }

    void KShader::SetUniform(const std::string& name, const bool& b) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), b);
    }

    void KShader::SetUniform(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void KShader::SetUniform(const std::string& name, const size_t& s) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<GLint>(s));
    }

    void KShader::SetUniform(const std::string& name, const int& i) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<GLint>(i));
    }

    void KShader::SetUniform(const std::string& name, const unsigned& i) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<GLint>(i));
    }

    void KShader::SetUniform(const std::string& name, const double& i) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<GLint>(i));
    }

    void KShader::SetUniform(const std::string& name, const glm::vec3* i) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 256, &i[0][0]);
    }

   
}
