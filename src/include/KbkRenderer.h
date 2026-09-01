#pragma once 

/**
 * @file    KRenderer.h
 * @brief   Renderer basic ckass and overrudes to update the pipelines automatically 
 *
 * @details  Render class that paints
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-09-25
 * @version 1.0
 */

#include <glm/vec2.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include "KbkStage.h"
#include "KbkBuffer.h"
#include "KbkShader.h"

namespace Graphics
{
    class KRenderer
    {
        public:
        /**
        * @brief  It constructs the renderer
        * @param[in] width: width of the window
        * @param[in] height: height of the window   
        */  
        KRenderer(int w, int h) : width_(w), height_(h){
            
        }
        
        /**
        * @brief  virtual destructor for the renderer   
        */
        virtual ~KRenderer() = default;
        
        /**
        * @brief  It overrides the drawing of each of the 
        */
        virtual void draw() = 0;
        
        protected:
         /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
        *         This variance creates an HDR color buffer (standard texture)
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @return void 
        */
        void subscribeBuffer(int width, int height, const std::string& key);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with various textures
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return void 
        */
        void subscribeBuffer(int width, int height, const std::string& key, std::vector<KAttachment>& attachments);

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
        void subscribeBuffer(int width, int height,  const std::string& key, KAttachment* attachment, int size);

        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with a single customized attachment
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments 
        * @return void 
        */
        void subscribeBuffer(int width, int height,  const std::string& key, KAttachment& attachment);

        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with various textures implicitly created
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return void 
        */
        void subscribeBuffer(int width, int height, const std::string& key, std::vector<GLAttachment>& attachments);

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
        void subscribeBuffer(int width, int height, const std::string& key, GLAttachment* attachments, int size);

        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with a single texture created implicitly
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return void 
        */
        void subscribeBuffer(int width, int height, const std::string& key, GLAttachment attachment);

        int width_, height_;
        std::unordered_map<std::string, std::unique_ptr<KShader>> shaders_; 
        std::unordered_map<std::string, std::unique_ptr<KBuffer>> buffers_; 
        std::vector<std::unique_ptr<KStage>> stages_;
    };

    class KForwardRenderer : public KRenderer
    {
        //KForwardRenderer();

        void draw() override;
    };

    class KInstancedRenderer : public KRenderer
    {

        //KInstancedRenderer();

        void draw() override;
    };

    class KDeferredRenderer : public KRenderer
    {
    public: 
        KDeferredRenderer(int width, int height);

        void draw() override;
    };

    class KRayTraceRenderer : public KRenderer
    {
        //KRayTraceRenderer();

        void draw() override;
    };
}