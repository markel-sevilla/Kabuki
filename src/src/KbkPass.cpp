/**
 * @file    Staee.h
 * @brief   Simple OpenGL pipeline wrap up class 
 *
 * @details  Pipeline class that implements the abstraction for a unit stage
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
#include "KbkPass.h"
#include "KbkShader.h"

namespace Graphics
{

    
    std::unique_ptr<KPass> KPass::createPass(const char* vPath, const char* fPath, PassType type)
    {
        switch(type)
        {
            case PassType::GPass: // create the GPass with its shader
                return std::unique_ptr<KPass>(static_cast<KPass*>(new GPass(new KShader(vPath, fPath))));
            
            default:
                return nullptr;
        }
    }

    KPass::KPass( KShader* shader ) : shader_(std::unique_ptr<KShader>(shader))
    {
    }

    GPass::GPass( KShader* shader ) : KPass::KPass(shader)    
    {
    }
   
    void GPass::execute()
    {
        bindFrameBuffer(buffers_["GBuffer"]->getHandle());
    }

    void KPass::bindFrameBuffer(GLuint buffer) const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	    glClearColor(0.0, 0.0, 0.0, 1.0); // keep it black so it doesn't leak 
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


    GLuint KPass::subscribeBuffer(int width, int height, const std::string& key)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height));

        return buffers_[key]->getHandle();
    }

    GLuint KPass::subscribeBuffer(int width, int height, const std::string& key, GLAttachment attachment)
    {
        KAttachment att(attachment);

        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, att));

        return buffers_[key]->getHandle();

    }

    GLuint KPass::subscribeBuffer(int width, int height, const std::string& key, GLAttachment* attachments, int size)
    {
        std::vector<KAttachment> att;

        //initialize the attachments
        for(int i = 0; i < size; i++)
            att.push_back(Graphics::KAttachment(attachments[i]));

        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, att));

        return buffers_[key]->getHandle();

    }

    GLuint KPass::subscribeBuffer(int width, int height, const std::string& key, std::vector<GLAttachment>& attachments)
    {
        std::vector<KAttachment> att;

        //initialize the attachments
        for(auto& gl: attachments)
            att.push_back(Graphics::KAttachment(gl));

        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, att));

        return buffers_[key]->getHandle();

    }


    GLuint KPass::subscribeBuffer(int width, int height, const std::string& key, KAttachment& attachment)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachment));

        return buffers_[key]->getHandle();
    }

    GLuint KPass::subscribeBuffer(int width, int height, const std::string& key, KAttachment* attachments, int size)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachments, size));

        return buffers_[key]->getHandle();
    }

    GLuint KPass::subscribeBuffer(int width, int height, const std::string& key, std::vector<KAttachment>& attachments)
    {
        buffers_.insert_or_assign(key, std::make_unique<KBuffer>(width, height, attachments));

        return buffers_[key]->getHandle();
    }
}