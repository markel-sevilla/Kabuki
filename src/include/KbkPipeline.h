#pragma once 

/**
 * @file    KPipeline.h
 * @brief   Simple OpenGL pipeline wrap up class 
 *
 * @details  Pipeline class that implements the abstraction for OpenGL to later be used in the stager
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */

#include <glm/vec2.hpp>
#include "KbkPipelineConfig.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include "KbkStage.h"

namespace Graphics
{
    class KStage;

    class KPipeline
    {
        public:
        /**
        * @brief  It constructs the pipeline itself  
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] resizeWindow: window resize yes or no
        * @param[in] hintVisible: hint visible yes or no
        * @return void 
        */
        
        KPipeline( int width, int height, bool resizeable = true, bool hintVisible = true);
        KPipelineConfig glConfig_;
        
        ~KPipeline();
     

        private:
        
        /**
        * @brief  It loads the glfw pipeline with glad  
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] resizeWindow: window resize yes or no
        * @param[in] hintVisible: hint visible yes or no
        * @return void 
        */
        int initGLFWPipeline(int width, int height, bool resizeWindow, bool hintVisible);
        std::unordered_map<int, std::unique_ptr<KStage>> u_stages_;
        std::vector<std::unique_ptr<KStage>> stages_;

        glm::vec2 size_;
        bool resizeable_;
        bool hintVisible_;
        
    };
}