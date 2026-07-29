#pragma once 

/**
 * @file    Pipeline.h
 * @brief   Simple OpenGL pipeline wrap up class 
 *
 * @details  Pipeline class that implements the abstraction for OpenGL to later be used in the stager
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */

 #include "KbkPipelineConfig.h"

namespace Graphics
{
    class KbkPipeline
    {
        public:
        /**
        * @brief  It loads the shader from a vertex and a fragment  
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] resizeWindow: window resize yes or no
        * @param[in] hintVisible: hint visible yes or no
        * @return void 
        */
        void initGLFWPipeline(int width, int height, bool resizeWindow, bool hintVisible);

        PipelineConfig glConfig_;


        private:

        std::unordered_map<int, KbkStage> u_stages_;
        std::vector<KbkStage> stages_;
    }
}