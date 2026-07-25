 
 /**
 * @file    ComputeShader.cpp
 * @brief   Wrap up for the compute shader OpenGl class 
 *
 * @details  It wraps up a shader and makes it easier fpr the pipeline to use it
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
 #include "ComputeShader.h"
 
 namespace Graphics
 {

    /**
    * @brief  It loads the shader from a vertex and a fragment  
    * @param[in] computePath: the compute shader path
    * @return void 
    */

    void ComputeShader::LoadComputeShader( const char* computePath)
    {

        std::string computeShaderCode;
        std::ifstream cShaderFile;

        // ensure ifstream objects can throw exceptions:
        cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            cShaderFile.open(computePath);
            std::stringstream cShaderStream;

            // read file's buffer contents into streams
            cShaderStream << cShaderFile.rdbuf();

            // close file handlers
            cShaderFile.close();

            // convert stream into string
            computeShaderCode = cShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::COMPUTE_SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
        }


        GLuint shader = glCreateShader(GL_COMPUTE_SHADER);

        const char* cComputeShaderCode = computeShaderCode.c_str();

        glShaderSource(shader, 1, &cComputeShaderCode, nullptr);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char log[1024];
            glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
            std::cerr << "Error compilando compute shader:\n" << log << std::endl;
            glDeleteShader(shader);
            return ;
        }

        ID = glCreateProgram();
        glAttachShader(ID, shader);
        glLinkProgram(ID);

        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char log[1024];
            glGetProgramInfoLog(ID, sizeof(log), nullptr, log);
            std::cerr << "Error enlazando compute shader:\n" << log << std::endl;
            glDeleteShader(shader);
            glDeleteProgram(ID);
            return ;
        }

        glDeleteShader(shader);

    }
 }
 