
#include "KbkRenderer.h"
#include "Log.h"

/**
 * @file    KRenderer.cpp
 * @brief   Renderer basic ckass and overrudes to update the pipelines automatically 
 *
 * @details  Render class that paints
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-09-25
 * @version 1.0
 */

 namespace Graphics
 {

    KDeferredRenderer::KDeferredRenderer(int width, int height) : KRenderer::KRenderer(width, height)
    {
        GLAttachment att[4] = {Graphics::GLAttachment::ColorHDR, Graphics::GLAttachment::ColorHDR, Graphics::GLAttachment::ColorLDR, Graphics::GLAttachment::Depth };
        
        //subscribe the pass 
        auto pass = subscribePass(shader, PassType::GPass);

        //subscribe the buffer with three attachments
        pass->subscribeBuffer(width_, height_, "GBuffer", att, 4);


        
        //Culling(avoid seen back faces of the objs)
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // to draw second pass draw again front faces
        glFrontFace(GL_CCW);
        
        //  enable test depths
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

    }

    std::unique_ptr<KPass> KRenderer::subscribePass(Shader* shader, PassType type)
    {
        passes_.push_back(createPass(shader, type_));

        return passes_.back();
    }

    void KDeferredRenderer::draw(const glm::mat4& view, const glm::mat4& proj)
    {

    }

    void KForwardRenderer::draw(const glm::mat4& view, const glm::mat4& proj)
    {

    }

    void KInstancedRenderer::draw(const glm::mat4& view, const glm::mat4& proj)
    {

    }

    void KRayTraceRenderer::draw(const glm::mat4& view, const glm::mat4& proj)
    {

    }

 }