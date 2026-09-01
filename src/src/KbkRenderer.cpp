
#include "KbkRenderer.h"

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
        GLAttachment att[3] = {Graphics::GLAttachment::ColorHDR, Graphics::GLAttachment::ColorHDR, Graphics::GLAttachment::ColorLDR};
        
        //subscribe the buffer with three attachments
        subscribeBuffer(width_, height_, "GBuffer", att, 3 );
        
        //Culling(avoid seen back faces of the objs)
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // to draw second pass draw again front faces
        glFrontFace(GL_CCW);
        
        //  enable test depths
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

    }
    void KDeferredRenderer::draw()
    {

    }

    void KForwardRenderer::draw()
    {

    }

    void KInstancedRenderer::draw()
    {

    }

    void KRayTraceRenderer::draw()
    {

    }

    void KRenderer::subscribeBuffer(int width, int height, const std::string& key)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height));
    }

    void KRenderer::subscribeBuffer(int width, int height, const std::string& key, GLAttachment attachment)
    {
        KAttachment att(attachment);

        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, att));
    }

    void KRenderer::subscribeBuffer(int width, int height, const std::string& key, GLAttachment* attachments, int size)
    {
        std::vector<KAttachment> att;

        //initialize the attachments
        for(int i = 0; i < size; i++)
            att.push_back(Graphics::KAttachment(attachments[size]));

        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, att));
    }

    void KRenderer::subscribeBuffer(int width, int height, const std::string& key, std::vector<GLAttachment>& attachments)
    {
        std::vector<KAttachment> att;

        //initialize the attachments
        for(auto& gl: attachments)
            att.push_back(Graphics::KAttachment(gl));

        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, att));
    }

    void KRenderer::subscribeBuffer(int width, int height, const std::string& key, KAttachment& attachment)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachment));
    }

    void KRenderer::subscribeBuffer(int width, int height, const std::string& key, KAttachment* attachments, int size)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachments, size));
    }

    void KRenderer::subscribeBuffer(int width, int height, const std::string& key, std::vector<KAttachment>& attachments)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachments));
    }
 }