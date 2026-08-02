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
#include "KbkBuffer.h"

namespace Graphics
{
    GLFormatInfo KBuffer::ResolveFormat(GLAttachment kind) {
        
        switch (kind) {

            case GLAttachment::ColorHDR:
                return { GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0 };
            case GLAttachment::ColorLDR:
                return { GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0 };
            case GLAttachment::ColorSingleF:
                return { GL_R32F, GL_RED, GL_FLOAT, GL_COLOR_ATTACHMENT0 };
            case GLAttachment::ColorRG_F:
                return { GL_RG16F, GL_RG, GL_FLOAT, GL_COLOR_ATTACHMENT0 };
            case GLAttachment::Depth:
                return { GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT };
            case GLAttachment::DepthStencil:
                return { GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, GL_DEPTH_STENCIL_ATTACHMENT };
            case GLAttachment::Stencil:
                return { GL_STENCIL_INDEX8, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, GL_STENCIL_ATTACHMENT };
        }
        return {};
    }


	KAttachment::KAttachment()
    KBuffer::KBuffer(int width, int height, const std::vector<GLFormatInfo>& attachments)
    {
        glGenFramebuffers(1, &b_handle_);
	    glBindFramebuffer(GL_FRAMEBUFFER , b_handle_);
        
		GLFormatInfo depth; 
		int i = 0;

        for(auto& att : attachments)
        {
			// depth case is handled later 
			if(att.attachmentPoint_ == GL_DEPTH_ATTACHMENT)
			{
				depth = att;
				continue;
			}

            // - position color buffer
	        glGenTextures(1, &att.t_handle_);
	        glBindTexture(GL_TEXTURE_2D, texture.t_handle_);
	        glTexImage2D(GL_TEXTURE_2D, 0, att.internalFormat_, width, height, 0, att.format, att.type_, NULL);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i++, GL_TEXTURE_2D, att.t_handle_, 0);
        }
		

    	// Tell OpenGL which color attachments we�ll use (of this framebuffer) for rendering
    	GLuint attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
    	glDrawBuffers(5, attachments);

    	// Create and attach depth buffer 
		glGenTextures(1, &gDepth);
		glBindTexture(GL_TEXTURE_2D, gDepth);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);

		// Check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cerr << "Framebuffer not complete!" << std::endl;

    	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    KBuffer(int width, int height, const KTextureParams& textures, const KDepthParam& depth = 0);
}