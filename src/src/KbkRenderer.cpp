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

    void KRenderer::subscribe_buffer(int width, int height, std::string& key)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height));
    }

    void KRenderer::subscribe_buffer(int width, int height, std::string& key, GLAttachment attachment)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, Graphics::KAttachment(attachment)));
    }

    void KRenderer::subscribe_buffer(int width, int height, std::string& key, GLAttachment* attachments, int size)
    {
        std::vector<KAttachment> att;

        //initialize the attachments
        for(int i = 0; i < size; i++)
            att.push_back(Graphics::KAttachment(attachments[size]));

        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, att));
    }

    void KRenderer::subscribe_buffer(int width, int height, std::string& key, std::vector<GLAttachment>& attachments)
    {
        std::vector<KAttachment> att;

        //initialize the attachments
        for(auto& gl: attachments)
            att.push_back(Graphics::KAttachment(gl));

        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, Graphics::KAttachment(att)));
    }

    void KRenderer::subscribe_buffer(int width, int height, std::string& key, KAttachment& attachment)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachment));
    }

    void KRenderer::subscribe_buffer(int width, int height, std::string& key, KAttachment* attachments, int size)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachments, size));
    }

    void KRenderer::subscribe_buffer(int width, int height, std::string& key, std::vector<KAttachment>& attachments)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachments));
    }
 }