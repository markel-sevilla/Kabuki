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
    KPass::KPass( KShader* shader ) : shader_(std::unique_ptr<KShader>(shader))
    {
    }

    GPass::GPass( KShader* shader ) : KPass::KPass(shader)    
    {
    }
   
    void GPass::execute(GLuint buffer)
    {

    }

    void KPass::bindFrameBuffer(GLuint buffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	    glClearColor(0.0, 0.0, 0.0, 1.0); // keep it black so it doesn't leak 
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}