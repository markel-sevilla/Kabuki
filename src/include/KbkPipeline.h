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

#include "KbkPass.h"
#include "KbkBuffer.h"
#include "KbkRenderer.h"

class GLFWwindow;
class Camera;

namespace Graphics
{
    class KPass;

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

        static std::unique_ptr<KPipeline> kCreatePipelineGL(int width, int height, KRenderType type = KRenderType::FWD , bool resizeable = true, bool hintVisible = true);
        static std::unique_ptr<KPipeline> kCreatePipelineVK(int width, int height, KRenderType type = KRenderType::FWD , bool resizeable = true, bool hintVisible = true);

        /**
        * @brief  It renders tehe current frame   
        * @param[in] width: GLFW camera 
        * @return void 
        */
        void render(const glm::mat4& view, const glm::mat4& proj );

        /**
        * @brief  sets the clear color   
        * @param[in] rgba: clear color for the window 
        * @return void 
        */

        void setClearColor(const glm::vec4& rgba);
        
        ~KPipeline();
        
        /**
        * @brief  returns the window   
        * @return void 
        */
        GLFWwindow* getWindow() const 
        {
            return window_;
        }

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
        GLFWwindow* window_; 
        bool resizeable_;
        bool hintVisible_;
        glm::vec4 clearColor_;
        
    };
}