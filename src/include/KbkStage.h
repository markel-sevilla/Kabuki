#pragma once 

/**
 * @file    KbkStage.h
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
   

    class KbkStage
    {
        public:
        /**
        * @brief  It loads the shader from a vertex and a fragment  
        * @param[in] shader: shader that is going to be executed
        * @return void 
        */

        KbkStage(Shader* shader);

        private:

        std::vector<>
    }
}