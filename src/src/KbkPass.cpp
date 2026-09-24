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
#include "KbkRenderer.h"
#include "KbkShader.h"
#include "Log.h"

namespace Graphics
{

    
    std::unique_ptr<KPass> KPass::createPass(KRenderer* renderer, const char* vPath, const char* fPath, PassType type)
    {
        switch(type)
        {
            case PassType::GPass: // create the GPass with its shader
                return std::unique_ptr<KPass>(static_cast<KPass*>(new GPass(renderer, new KShader(vPath, fPath))));
            
            default:
                return nullptr;
        }
    }

    KPass::KPass( KRenderer* renderer, KShader* shader ) : owner_(renderer), shader_(std::unique_ptr<KShader>(shader))
    {
    }

    GPass::GPass( KRenderer* renderer, KShader* shader ) : KPass::KPass(renderer, shader)    
    {
        LOG_TRACE("Geometry Pass was succesfully created");
    }
   
    void GPass::execute(const glm::mat4& view, const glm::mat4& proj) const
    {
        auto& buffer = buffers_.at("GBuffer");
        bindFrameBuffer(buffer->getHandle());

        shader_->Use();
	
	    for (auto& mesh : getMeshes())
	    {
	    	glBindVertexArray(mesh->getVAO());
        
	    	for (size_t i = 0; i < mesh->getM2Ws().size(); i++)
	    	{
            
	    			shader_->SetUniform("avgNormals", /*useAvgNormals*/false);

	    			//pass the matrices
	    			shader_->SetUniform("model", mesh->getM2Ws().at(i));
	    			shader_->SetUniform("proj", proj);
	    			shader_->SetUniform("view", view);
            
	    			if (mesh->getTexture())
	    			{
	    				glActiveTexture(GL_TEXTURE0); // Diffuse
	    				shader_->SetUniform("baseTexture", 0);
	    				glBindTexture(GL_TEXTURE_2D, mesh->getTexture()->getBaseID());
                    
	    				glActiveTexture(GL_TEXTURE1); // Normal map
	    				shader_->SetUniform("normalMap", 1);
	    				glBindTexture(GL_TEXTURE_2D, mesh->getTexture()->getNormalID());
                    
	    				glActiveTexture(GL_TEXTURE2); //specular Texture
	    				shader_->SetUniform("specularTex", 2);
	    				glBindTexture(GL_TEXTURE_2D, mesh->getTexture()->getMetallicID());
	    			}
                
	    		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->getIndices().size()), GL_UNSIGNED_INT, (void*)0);
	    	}
        
	    }
    
	    //go back to default
	    glActiveTexture(GL_TEXTURE0);
	    glBindVertexArray(0);
	    glUseProgram(0);
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

    const std::vector<std::unique_ptr<KMesh>>& KPass::getMeshes() const
    {
        return owner_->getMeshes();
    }
}