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
#include <iostream>
#include "Log.h"

namespace Graphics
{
    GLFormatInfo KAttachment::ResolveFormat(GLAttachment kind) {
        
        switch (kind) {

            case GLAttachment::ColorHDR:
                return { GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, false };
            case GLAttachment::ColorLDR:
                return { GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0, false };
            case GLAttachment::ColorSingleF:
                return { GL_R32F, GL_RED, GL_FLOAT, GL_COLOR_ATTACHMENT0, true };
            case GLAttachment::ColorRG_F:
                return { GL_RG16F, GL_RG, GL_FLOAT, GL_COLOR_ATTACHMENT0, false };
            case GLAttachment::Depth:
                return { GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT, true };
            case GLAttachment::DepthStencil:
                return { GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, GL_DEPTH_STENCIL_ATTACHMENT, true };
            case GLAttachment::Stencil:
                return { GL_STENCIL_INDEX8, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, GL_STENCIL_ATTACHMENT, true };
        }
		
        return {};
    }

	KBuffer::KBuffer(int width, int height) 
    {
        glGenFramebuffers(1, &handle_);
	    glBindFramebuffer(GL_FRAMEBUFFER , handle_);
        

		std::vector<GLuint> attachmentsToDraw;
		
		KAttachment att;

		//standard color buffer 
	    glGenTextures(1, &att.getHandle());
	    glBindTexture(GL_TEXTURE_2D, att.getHandle());

		glTexImage2D(GL_TEXTURE_2D, 0, att.getFormat().internalFormat_, width, height, 0, att.getFormat().format_, att.getFormat().type_, NULL);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		//see if the attachment is clamped 
		if(att.getFormat().clamped)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, att.getHandle(), 0);
		
		// push back the attachments to later draw them
		attachmentsToDraw.push_back(GL_COLOR_ATTACHMENT0);
		
		// Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    	glDrawBuffers(attachmentsToDraw.size(), &attachmentsToDraw[0]);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		
		// Check if framebuffer is complete
		if ( status != GL_FRAMEBUFFER_COMPLETE)
			isFrameBufferComplete(status);

    	glBindFramebuffer(GL_FRAMEBUFFER, 0);

		attachments_.push_back(att);

		LOG_TRACE("KBuffer was succesfully created");
	}
	
    KBuffer::KBuffer(int width, int height, std::vector<KAttachment>& attachments)
    {
		if(attachments.empty()) return;

        glGenFramebuffers(1, &handle_);
	    glBindFramebuffer(GL_FRAMEBUFFER, handle_);
        
		int depth = -1; 

		int i = 0;
		std::vector<GLuint> attachmentsToDraw;
		
		
        for(auto& att : attachments)
        {
			// depth case is handled later 
			if(att.getFormat().format_ == GL_DEPTH_COMPONENT)
			{
				depth = i;
				continue;
			}

			//standard color buffer 
	        glGenTextures(1, &att.getHandle());
	        glBindTexture(GL_TEXTURE_2D, att.getHandle());
	        glTexImage2D(GL_TEXTURE_2D, 0, att.getFormat().internalFormat_, width, height, 0, att.getFormat().format_, att.getFormat().type_, NULL);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
			//see if the attachment is clamped 
			if(att.getFormat().clamped)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			
	        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, att.getHandle(), 0);

			// push back the attachments to later draw them
			attachmentsToDraw.push_back(GL_COLOR_ATTACHMENT0 + i++);
        }
		
    	// Tell OpenGL which color attachments well use (of this framebuffer) for rendering
    	glDrawBuffers(attachmentsToDraw.size(), &attachmentsToDraw[0]);

    	// Create and attach depth buffer 
		if(depth != -1 && attachments[depth].getFormat().attachmentPoint_ == GL_DEPTH_ATTACHMENT)
		{
			glGenTextures(1, &attachments[depth].getHandle());
			glBindTexture(GL_TEXTURE_2D, attachments[depth].getHandle());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attachments[depth].getHandle(), 0);
		}
		
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		// Check if framebuffer is complete
		if ( status != GL_FRAMEBUFFER_COMPLETE)
			isFrameBufferComplete(status);

    	glBindFramebuffer(GL_FRAMEBUFFER, 0);

		attachments_ = attachments;

		LOG_TRACE("KBuffer was succesfully created");

    }

	KBuffer::~KBuffer()
	{
		attachments_.clear();
		
		glDeleteBuffers(1, &handle_);

		handle_ = 0;
	}

	KBuffer::KBuffer(int width, int height, KAttachment* attachments, int size)
    {
		if(size <= 0) return;
		
        glGenFramebuffers(1, &handle_);
	    glBindFramebuffer(GL_FRAMEBUFFER , handle_);
        

		KAttachment depth; 
		std::vector<GLuint> attachmentsToDraw;
		
        for(int i = 0; i < size; i++)
        {
			KAttachment& att = attachments[i];

			// depth case is handled later 
			if(att.getFormat().attachmentPoint_ == GL_DEPTH_ATTACHMENT)
			{
				depth = att;
				continue;
			}

			//standard color buffer 
	        glGenTextures(1, &att.getHandle());
	        glBindTexture(GL_TEXTURE_2D, att.getHandle());
	        glTexImage2D(GL_TEXTURE_2D, 0, att.getFormat().internalFormat_, width, height, 0, att.getFormat().format_, att.getFormat().type_, NULL);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
			//see if the attachment is clamped 
			if(att.getFormat().clamped)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			
	        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, att.getHandle(), 0);

			// push back the attachments to later draw them
			attachmentsToDraw.push_back(GL_COLOR_ATTACHMENT0 + i);
			
			attachments_.push_back(att);
        }
		
    	// Tell OpenGL which color attachments we�ll use (of this framebuffer) for rendering
    	glDrawBuffers(attachmentsToDraw.size(), &attachmentsToDraw[0]);

    	// Create and attach depth buffer 
		if(depth.getFormat().attachmentPoint_ == GL_DEPTH_ATTACHMENT)
		{
			glGenTextures(1, &depth.getHandle());
			glBindTexture(GL_TEXTURE_2D, depth.getHandle());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth.getHandle(), 0);

			attachments_.push_back(depth);
		}
		

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		// Check if framebuffer is complete
		if ( status != GL_FRAMEBUFFER_COMPLETE)
			isFrameBufferComplete(status);


    	glBindFramebuffer(GL_FRAMEBUFFER, 0);

		LOG_TRACE("KBuffer was succesfully created");

    }

    KBuffer::KBuffer(int width, int height, KAttachment& att)
	{
		glGenFramebuffers(1, &handle_);
	    glBindFramebuffer(GL_FRAMEBUFFER , handle_);
		
		//standard color buffer 
	    glGenTextures(1, &att.getHandle());
	    glBindTexture(GL_TEXTURE_2D, att.getHandle());
	    glTexImage2D(GL_TEXTURE_2D, 0, att.getFormat().internalFormat_, width, height, 0, att.getFormat().format_, att.getFormat().type_, NULL);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//see if the attachment is clamped 
		if(att.getFormat().clamped)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, att.getHandle(), 0);
		
    	// Tell OpenGL which color attachments we�ll use (of this framebuffer) for rendering
		GLuint attachment = GL_COLOR_ATTACHMENT0;
    	glDrawBuffers(1, &attachment);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		// Check if framebuffer is complete
		if (status != GL_FRAMEBUFFER_COMPLETE)
			isFrameBufferComplete(status);


		attachments_.push_back(att);
    	glBindFramebuffer(GL_FRAMEBUFFER, 0);

		LOG_TRACE("KBuffer was succesfully created");

	}

	void KBuffer::isFrameBufferComplete(GLenum status)
	{
		switch (status)
		{
		    case GL_FRAMEBUFFER_UNDEFINED:
		        LOG_ERROR("GL_FRAMEBUFFER_UNDEFINED");
		        break;
		
		    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		        LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		        break;
		
		    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		        LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		        break;
		
		    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		        LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
		        break;
		
		    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		        LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
		        break;
		
		    case GL_FRAMEBUFFER_UNSUPPORTED:
		        LOG_ERROR("GL_FRAMEBUFFER_UNSUPPORTED");
		        break;
		}
	}
}