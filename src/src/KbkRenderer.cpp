
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
        
        //subscribe the buffer with three attachments
        GLuint handleToBuffer = subscribeBuffer(width_, height_, "GBuffer", att, 4);

        
        
        //Culling(avoid seen back faces of the objs)
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // to draw second pass draw again front faces
        glFrontFace(GL_CCW);
        
        //  enable test depths
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

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

    GLuint KRenderer::subscribeBuffer(int width, int height, const std::string& key)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height));

        return buffers_[key]->getHandle();
    }

    GLuint KRenderer::subscribeBuffer(int width, int height, const std::string& key, GLAttachment attachment)
    {
        KAttachment att(attachment);

        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, att));

        return buffers_[key]->getHandle();

    }

    GLuint KRenderer::subscribeBuffer(int width, int height, const std::string& key, GLAttachment* attachments, int size)
    {
        std::vector<KAttachment> att;

        //initialize the attachments
        for(int i = 0; i < size; i++)
            att.push_back(Graphics::KAttachment(attachments[i]));

        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, att));

        return buffers_[key]->getHandle();

    }

    GLuint KRenderer::subscribeBuffer(int width, int height, const std::string& key, std::vector<GLAttachment>& attachments)
    {
        std::vector<KAttachment> att;

        //initialize the attachments
        for(auto& gl: attachments)
            att.push_back(Graphics::KAttachment(gl));

        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, att));

        return buffers_[key]->getHandle();

    }


    GLuint KRenderer::subscribeBuffer(int width, int height, const std::string& key, KAttachment& attachment)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachment));

        return buffers_[key]->getHandle();
    }

    GLuint KRenderer::subscribeBuffer(int width, int height, const std::string& key, KAttachment* attachments, int size)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachments, size));

        return buffers_[key]->getHandle();
    }

    GLuint KRenderer::subscribeBuffer(int width, int height, const std::string& key, std::vector<KAttachment>& attachments)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachments));

        return buffers_[key]->getHandle();
    }
 }