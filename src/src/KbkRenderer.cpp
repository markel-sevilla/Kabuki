
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
        passes_.push_back(KPass::createPass(this, vPath, fPath, type));

        return getLastPass();
    }

    const std::vector<std::unique_ptr<KMesh>>& KRenderer::getMeshes() const
    {
        return meshes_;
    }

    void KDeferredRenderer::render(const glm::mat4& view, const glm::mat4& proj)
    {
        //render all the passes from the renderer
        for(auto& pass: passes_)
            pass->execute(view, proj);

    }

    void KForwardRenderer::render(const glm::mat4& view, const glm::mat4& proj)
    {

    }

    void KInstancedRenderer::render(const glm::mat4& view, const glm::mat4& proj)
    {

    }

    void KRayTraceRenderer::render(const glm::mat4& view, const glm::mat4& proj)
    {

    }

    void KRenderer::addMesh( KMesh* mesh)
    {
        meshes_.push_back(std::unique_ptr<KMesh>(mesh));
    }
 }