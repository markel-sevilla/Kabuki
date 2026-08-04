#pragma once 

/**
 * @file    KStage.h
 * @brief   Unitary stage for the pipeline 
 *
 * @details  one stage ususally represents one execution of a certain shader and its parameters
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */


namespace Graphics
{
   
    class Shader;
    class KStage
    {
        public:
        /**
        * @brief  It loads the shader from a vertex and a fragment  
        * @param[in] shader: shader that is going to be executed
        * @return void 
        */

        KStage(Shader* shader);

        private:
    };
}