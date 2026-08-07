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

         /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
        *         This variance creates an HDR color buffer (standard texture)
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @return void 
        */
        void subscribeBuffer(int width, int height, std::string& key);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with various textures
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return void 
        */
        void subscribeBuffer(int width, int height, std::string& key, std::vector<KAttachment>& attachments);

        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with various textures explicitly created through the structs
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @param[in] size: number of attachments
        * @return void 
        */
        void subscribeBuffer(int width, int height,  std::string& key, KAttachment* attachment, int size);

        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with a single customized attachment
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments 
        * @return void 
        */
        void subscribeBuffer(int width, int height,  std::string& key, KAttachment& attachment);

        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with various textures implicitly created
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return void 
        */
        void subscribeBuffer(int width, int height,  std::string& key, std::vector<GLAttachment>& attachments);

        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with various textures implicitly created
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @param[in] size: number of attachments
        * @return void 
        */
        void subscribeBuffer(int width, int height,  std::string& key, GLAttachment* attachments, int size);

        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with a single texture created implicitly
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return void 
        */
        void subscribeBuffer(int width, int height,  std::string& key, GLAttachment attachment);
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

        std::unordered_map<std::string, std::unique_ptr<KBuffer>> buffers_; 
        std::vector<std::unique_ptr<KStage>> stages_;

        glm::vec2 size_;
        bool resizeable_;
        bool hintVisible_;
        
    };
}