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
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"

namespace Graphics
{
    std::unique_ptr<KPass> KPass::createPass(KRenderer* renderer, const char* vPath, const char* fPath, PassType type)
    {
        switch(type)
        {
            case PassType::GPass: // create the GPass with its shader
                return std::unique_ptr<KPass>(static_cast<KPass*>(new GPass(renderer, new KShader(vPath, fPath))));
            
            case PassType::AmbientPass: // create the Ambient Pass with its shader
                return std::unique_ptr<KPass>(static_cast<KPass*>(new AmbientPass(renderer, new KShader(vPath, fPath))));
            
            case PassType::LightingPass: // create the Lighting with its shader
                return std::unique_ptr<KPass>(static_cast<KPass*>(new LightingPass(renderer, new KShader(vPath, fPath))));

			case PassType::DisplayPass: // create the Display Pass with its shader
                return std::unique_ptr<KPass>(static_cast<KPass*>(new DisplayPass(renderer, new KShader(vPath, fPath))));
            
            default:
                return nullptr;
        }
    }

    KPass::KPass( KRenderer* renderer, KShader* shader ) : owner_(renderer), shader_(std::unique_ptr<KShader>(shader))
    {
    }

	KPass::~KPass()
	{
		buffers_.clear();
		owner_ = nullptr;
	}

	void KPass::renderQuad() const
	{
		glBindVertexArray(owner_->getQuadHandle());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

    GPass::GPass( KRenderer* renderer, KShader* shader ) : KPass::KPass(renderer, shader)    
    {
        LOG_TRACE("Geometry Pass was succesfully created");
    }
   
    void GPass::execute(const glm::mat4& view, const glm::mat4& proj) const
    {
        auto& buffer = buffers_.at("GBuffer");
        bindFrameBuffer(buffer->getHandle(), true);

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

    AmbientPass::AmbientPass( KRenderer* renderer, KShader* shader ) : KPass::KPass(renderer, shader)    
    {
        LOG_TRACE("Ambient Pass was succesfully created");
    }
   
    void AmbientPass::execute(const glm::mat4& view, const glm::mat4& proj) const
    {
        blitBuffers(0, buffers_.at("Pixel")->getHandle(), false);
        
	    // Enable blending
	    glEnable(GL_BLEND);
	    // Set additive blending
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	    shader_->Use();

	    //activate the textures of the g buffer
	    glActiveTexture(GL_TEXTURE0);
	    shader_->SetUniform("diffuseTex", 0);
	    glBindTexture(GL_TEXTURE_2D, buffers_.at("GBuffer")->getAttachmentHandle(2));

	    shader_->SetUniform("occlussion", false);
        
	    // if (!bilateralFilter)
	    // {
	    // 	glActiveTexture(GL_TEXTURE1);
	    // 	shader->SetUniform("ambientMask", 1);
	    // 	glBindTexture(GL_TEXTURE_2D, ambientOclussionMask);
	    // }
	    // else
	    // {
	    // 	glActiveTexture(GL_TEXTURE1);
	    // 	shader->SetUniform("ambientMask", 1);
	    // 	glBindTexture(GL_TEXTURE_2D, bilateralFilteredImage);
	    // }
    

	    shader_->SetUniform("ambient", 0.25f);

	    renderQuad();

	    glBindVertexArray(0);
	    glUseProgram(0);
    }

    LightingPass::LightingPass( KRenderer* renderer, KShader* shader ) : KPass::KPass(renderer, shader)    
    {
        LOG_TRACE("Lighting Pass was succesfully created");
    }
   

    void LightingPass::execute(const glm::mat4& view, const glm::mat4& proj) const
    {
        //TODO : put a check so that if the previous buffer and the current one are the same we don't chwck 
        auto& buffer = buffers_.at("GBuffer");
        //blitBuffers(0, buffers_.at("Pixel")->getHandle(), false); // change to pixel buffer 

		// Disable depth writes 
	    glDepthMask(GL_FALSE);
	    glDepthFunc(GL_GREATER);

	    // Enable face culling
	    glEnable(GL_CULL_FACE);

	    glCullFace(GL_FRONT); // to draw second pass draw again front faces
        
	    shader_->Use();
        
	    //activate the textures of the g buffer
	    glActiveTexture(GL_TEXTURE0);
	    shader_->SetUniform("posTex", 0);
	    glBindTexture(GL_TEXTURE_2D, buffer->getAttachmentHandle(0)); // TODO: pos id, have to change
        
	    glActiveTexture(GL_TEXTURE1);
	    shader_->SetUniform("normalTex", 1);
	    glBindTexture(GL_TEXTURE_2D, buffer->getAttachmentHandle(1)); // TODO: normal id, have to change
        
	    glActiveTexture(GL_TEXTURE2);
	    shader_->SetUniform("diffuseTex", 2);
	    glBindTexture(GL_TEXTURE_2D, buffer->getAttachmentHandle(2)); // TODO: diffuse id, have to change 
        
	    //do the light pass
	    for (int i = 0; i < getLights().size(); i++)
	    {
			
			auto light = getLights().at(i)->getType() == KLightType::Point ? 
						 static_cast<KPointLight*>(getLights().at(i).get()) : nullptr;
			
			if(light == nullptr)
				continue;

	    	glBindVertexArray(light->getSphere()->getVAO());

	    	//create the m2w for the sphere of the light
	    	glm::mat4 m2w = glm::translate(glm::mat4(1), glm::vec3(glm::translate(glm::mat4(1), glm::vec3(light->getMovementRadius() * sin(light->getMotionAngle()))) * glm::vec4(light->getPosition(), 1))) 
	    		          * glm::scale(glm::mat4(1), glm::vec3(2.1f * light->getRadius()));

	    	////pass the matrices
	    	shader_->SetUniform("proj", proj);
	    	shader_->SetUniform("model", m2w);
	    	shader_->SetUniform("view", view);

	    	//we pass the light properties
	    	shader_->SetUniform("light.pos", glm::vec3(view 
	    		                          * glm::translate(glm::mat4(1), glm::vec3(light->getMovementRadius() * sin(light->getMotionAngle()))) 
	    		                          * glm::vec4(light->getPosition(), 1)));

	    	shader_->SetUniform("light.color", glm::vec3(light->getColor()));
	    	shader_->SetUniform("light.radius", light->getRadius() );
            
	    	// if (mAnimateLights)
	    	// {
	    	// 	mLights[i]->motionAngle += 0.0016f;
	    	// }

	    	// we reconstruct the camera position from the view matrix
	    	shader_->SetUniform("viewPos", glm::vec3(glm::inverse(view)[3]));
        
	    	shader_->SetUniform("width", 1600.f);
	    	shader_->SetUniform("height", 900.f);

	    	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(light->getSphere()->getIndices().size()), GL_UNSIGNED_INT, (void*)0);

	    }

	    //for lighting optimization disable face culling	
	    glCullFace(GL_BACK); // to draw second pass draw again front faces
	    glFrontFace(GL_CCW);

	    glDepthMask(GL_TRUE);
	    glDepthFunc(GL_LEQUAL);
	    glBindVertexArray(0);
	    glUseProgram(0);
    }

	DisplayPass::DisplayPass( KRenderer* renderer, KShader* shader ) : KPass::KPass(renderer, shader)    
    {
        LOG_TRACE("Display Pass was succesfully created");
    }

	void DisplayPass::execute(const glm::mat4& view, const glm::mat4& proj) const
	{
		blitBuffers(buffers_.at("Pixel")->getHandle(), 0, false);

		auto pixelImage = buffers_.at("Pixel")->getAttachmentHandle(0);

		shader_->Use();
		//shader->SetUniform("bloomActive", mBloom);

		//activate the textures of the semifinalImage
		// glActiveTexture(GL_TEXTURE0);
		// shader->SetUniform("brightTexture", 0);
		// glBindTexture(GL_TEXTURE_2D, bloomedImage[1]);

		//activate the textures of the semifinalImage
		glActiveTexture(GL_TEXTURE0);
		shader_->SetUniform("pixelImage", 0);
		//for pencil render
		
		//glBindTexture(GL_TEXTURE_2D, distortedTexture[2]);
		glBindTexture(GL_TEXTURE_2D, pixelImage);

		renderQuad();

		//go back to default
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

    void KPass::blitBuffers(const unsigned int src, const unsigned int dst, bool copy) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, dst);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (copy)
		{
			// copy contents
			glBindFramebuffer(GL_READ_FRAMEBUFFER, src);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst); // write to default framebuffer
			
			//TODO : change this so that is not fixed
		    glBlitFramebuffer(0, 0, 1600, 900, 0, 0, 1600, 900, GL_DEPTH_BUFFER_BIT, GL_NEAREST); 
		}

		glBindFramebuffer(GL_FRAMEBUFFER, dst);
	}

    void KPass::bindFrameBuffer(GLuint buffer, bool clear) const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
		if(clear)
		{
			glClearColor(0.0, 0.0, 0.0, 1.0); // keep it black so it doesn't leak 
	    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
    }

    void KPass::attachBuffer(const std::string& key, KBuffer* buffer)
	{
		if(buffers_.find(key) != buffers_.end())
			return;

		buffers_.insert_or_assign(key, buffer);
	}


    const std::vector<std::unique_ptr<KMesh>>& KPass::getMeshes() const
    {
        return owner_->getMeshes();
    }

    const std::vector<std::unique_ptr<KLight>>& KPass::getLights() const
    {
        return owner_->getLights();
    }

    unsigned KPass::getShaderID() const
	{
		return shader_->ID;
	}

    const KBuffer* KPass::getBuffer(const std::string& key) const 
	{
		if(buffers_.find(key) == buffers_.end())
			return nullptr;

		return buffers_.at(key);
	}

}