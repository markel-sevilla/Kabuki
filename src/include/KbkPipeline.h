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
#include <vector>
#include <unordered_map>
#include <memory>
#include "KbkPipelineConfig.h"
#include "KbkStage.h"
#include "KbkBuffer.h"
#include "KbkRenderer.h"

namespace Graphics
{
    class KStage;

    enum class KAPI
    {
        GL,
        VK,
        TOTAL
    };

    enum class KRenderType
    {
        FWD,
        INS,
        DEF,
        RTC,
        TOTAL
    };

    class KPipeline
    {

        public:

        static KPipeline* kCreatePipelineGL(int width, int height, KRenderType type = KRenderType::FWD , bool resizeable = true, bool hintVisible = true);
        static KPipeline* kCreatePipelineVK(int width, int height, KRenderType type = KRenderType::FWD , bool resizeable = true, bool hintVisible = true);

        ~KPipeline();

        private:


        /**
        * @brief  It constructs the pipeline itself  
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] resizeWindow: window resize yes or no
        * @param[in] hintVisible: hint visible yes or no
        * @return void 
        */
        
        KPipeline( int width, int height, KAPI api = KAPI::GL, KRenderType type = KRenderType::FWD , bool resizeable = true, bool hintVisible = true);
        
        /**
        * @brief  It loads the glfw pipeline with glad  
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] resizeWindow: window resize yes or no
        * @param[in] hintVisible: hint visible yes or no
        * @return void 
        */

        int initGLFWPipeline(int width, int height, bool resizeWindow, bool hintVisible);

        /**
        * @brief  It creates the forward gl pipeline  
        * @return void 
        */

        void createForwardGL();

        /**
        * @brief  It creates the instanced gl pipeline  
        * @return void 
        */
        void createInstancedGL();

        /**
        * @brief  It creates the deferred gl pipeline  
        * @return void 
        */
        void createDeferredGL();

        /**
        * @brief  It creates the ray tracer gl pipeline  
        * @return void 
        */
        void createRayTraceGL();

        std::unique_ptr<KRenderer> renderer_;
        glm::ivec2 size_;
        KAPI api_;
        KRenderType type_;
        bool resizeable_;
        bool hintVisible_;
        
    };
}