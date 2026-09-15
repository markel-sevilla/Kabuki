
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
        auto pass = subscribePass("data/shaders/geometryVertex.vs", "data/shaders/geometryFragment.fs", PassType::GPass);

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

    KPass* KRenderer::getLastPass() const 
    {
        return passes_.back().get();
    }

    KPass* KRenderer::subscribePass(const char* vPath, const char* fPath, PassType type)
    {
        passes_.push_back(KPass::createPass(vPath, fPath, type));

        return getLastPass();
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