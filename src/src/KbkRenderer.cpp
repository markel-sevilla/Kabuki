
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
        //positions, normal, diffuse, depth
       // GLAttachmentKey key[4] = { GLAttachmentKey::PositionAtt, GLAttachmentKey::NormalAtt, GLAttachmentKey::DiffuseAtt, GLAttachmentKey::DepthAtt };
        GLAttachment att[4] = {Graphics::GLAttachment::ColorHDR, Graphics::GLAttachment::ColorHDR, Graphics::GLAttachment::ColorLDR, Graphics::GLAttachment::Depth };
        
        //subscribe the buffer with four attachments
        auto buffer = subscribeBuffer(width_, height_, att, 4);
        
        //subscribe the pass 
        auto pass = subscribePass("data/shaders/geometryVertex.vs", "data/shaders/geometryFragment.fs", PassType::GPass);

        //TODO : change the string based key system to a enum based key system
        pass->attachBuffer("GBuffer", buffer);

        GLAttachment pixelatt[2] = {Graphics::GLAttachment::ColorHDR, Graphics::GLAttachment::Depth };
        
        auto pixel = subscribeBuffer(width_, height_, pixelatt, 2);
        
        pass = subscribePass("data/shaders/ambientVertex.vs", "data/shaders/ambientFragment.fs", PassType::AmbientPass);
       
        pass->attachBuffer("GBuffer", buffer);

        pass->attachBuffer("Pixel", pixel);

        //subscribe the pass 
        pass = subscribePass("data/shaders/lightVertex.vs", "data/shaders/lightFragment.fs", PassType::LightingPass);

        pass->attachBuffer("GBuffer", buffer);

        pass->attachBuffer("Pixel", pixel);

        pass = subscribePass("data/shaders/displayVertex.vs", "data/shaders/displayFragment.fs", PassType::DisplayPass);

        pass->attachBuffer("Pixel", pixel);

         //Culling(avoid seen back faces of the objs)
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // to draw second pass draw again front faces
        glFrontFace(GL_CCW);
        
        //  enable test depths
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void KRenderer::generateQuad()
	{
		quadVAO_ = 0;
		quadVBO_ = 0;

		std::vector<float> quadVertices =
		{
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		// setup plane VAO
		glGenVertexArrays(1, &quadVAO_);
		glGenBuffers(1, &quadVBO_);
		glBindVertexArray(quadVAO_);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quadVertices.size(), quadVertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	}

    KCamera* KRenderer::getCam() const
    {
        return cam_.get();
    }

    void KRenderer::setCam(std::unique_ptr<Graphics::KCamera> cam)
    {
        cam_ = std::move(cam);
    }

    KPass* KRenderer::getLastPass() const 
    {
        return passes_.back().get();
    }

    KBuffer* KRenderer::getLastBuffer() const 
    {
        return buffers_.back().get();
    }
    
    KPass* KRenderer::subscribePass(const char* vPath, const char* fPath, PassType type)
    {
        passes_.push_back(KPass::createPass(this, vPath, fPath, type));

        return getLastPass();
    }


    KBuffer* KRenderer::subscribeBuffer(int width, int height)
    {
        buffers_.push_back( std::make_unique<KBuffer>(width, height));

        return getLastBuffer();
    }

    KBuffer* KRenderer::subscribeBuffer(int width, int height, GLAttachment attachment)
    {
        KAttachment att(attachment);

        buffers_.push_back(  std::make_unique<KBuffer>(width, height, att));

        return getLastBuffer();

    }

    KBuffer* KRenderer::subscribeBuffer(int width, int height,  GLAttachment* attachments, int size)
    {
        std::vector<KAttachment> att;

        //initialize the attachments
        for(int i = 0; i < size; i++)
            att.push_back(Graphics::KAttachment(attachments[i]));

        buffers_.push_back(  std::make_unique<KBuffer>(width, height, att));

        return getLastBuffer();

    }

    KBuffer* KRenderer::subscribeBuffer(int width, int height,  std::vector<GLAttachment>& attachments)
    {
        std::vector<KAttachment> att;

        //initialize the attachments
        for(auto& gl: attachments)
            att.push_back(Graphics::KAttachment(gl));

        buffers_.push_back( std::make_unique<KBuffer>(width, height, att));

        return getLastBuffer();

    }


    KBuffer* KRenderer::subscribeBuffer(int width, int height,  KAttachment& attachment)
    {
        buffers_.push_back( std::make_unique<KBuffer>(width, height, attachment));

        return getLastBuffer();
    }

    KBuffer* KRenderer::subscribeBuffer(int width, int height,  KAttachment* attachments, int size)
    {
        buffers_.push_back( std::make_unique<KBuffer>(width, height, attachments, size));

        return getLastBuffer();
    }

    KBuffer* KRenderer::subscribeBuffer(int width, int height, std::vector<KAttachment>& attachments)
    {
        buffers_.push_back( std::make_unique<KBuffer>(width, height, attachments));

        return getLastBuffer();
    }

    const std::vector<std::unique_ptr<KMesh>>& KRenderer::getMeshes() const
    {
        return meshes_;
    }

    void KRenderer::addMatrixToMesh(int index, const glm::mat4& matrix)
    {
        meshes_[index]->addM2W(matrix);
    }

    //TODO: make the renders constant
    void KDeferredRenderer::render(const glm::mat4& view, const glm::mat4& proj)
    {
        cam_->update();
        
        //render all the passes from the renderer
        for(int i = 0; i < passes_.size(); i++)
        {
            if(passes_.at(i).get())
            {
                passes_.at(i)->execute(view, proj);
            }
        }
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

    void KRenderer::addLight( KLight* light)
    {
        lights_.push_back(std::unique_ptr<KLight>(light));
    }


    const std::vector<std::unique_ptr<KLight>>& KRenderer::getLights() const
    {
        return lights_;
    }

 }