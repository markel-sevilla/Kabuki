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
#include "KbkPass.h"


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
        virtual void draw(const glm::mat4& view, const glm::mat4& proj) = 0;
        
        std::unique_ptr<KPass> subscribePass(Shader* shader, PassType type_);
        
        protected:
        int width_, height_;
        std::vector<std::unique_ptr<KPass>> passes_;
    };

    class KForwardRenderer : public KRenderer
    {
        //KForwardRenderer();

        void draw(const glm::mat4& view, const glm::mat4& proj) override;
    };

    class KInstancedRenderer : public KRenderer
    {

        //KInstancedRenderer();

        void draw(const glm::mat4& view, const glm::mat4& proj) override;
    };

    class KDeferredRenderer : public KRenderer
    {
    public: 
        KDeferredRenderer(int width, int height);

        void draw(const glm::mat4& view, const glm::mat4& proj) override;
    };

    class KRayTraceRenderer : public KRenderer
    {
        //KRayTraceRenderer();

        void draw(const glm::mat4& view, const glm::mat4& proj) override;
    };
}