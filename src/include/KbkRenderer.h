#pragma once 

/**
 * @file    KRenderer.h
 * @brief   Renderer basic ckass and overrudes to update the pipelines automatically 
 *
 * @details  Render class that paints
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-09-25
 * @version 1.0
 */

#include <glm/vec2.hpp>
#include <vector>
#include <memory>
#include "KbkPass.h"
#include "KbkMesh.h"
#include "KbkLight.h"
#include "KbkCamera.h"

namespace Graphics
{
    class KRenderer
    {
        public:
        /**
        * @brief  It constructs the renderer
        * @param[in] width: width of the window
        * @param[in] height: height of the window   
        */  
        KRenderer(int w, int h) : width_(w), height_(h){
            
            generateQuad();
        }
        
        /**
        * @brief  virtual destructor for the renderer   
        */
        virtual ~KRenderer() 
        {
            glDeleteBuffers(1, &quadVAO_);
            glDeleteBuffers(1, &quadVBO_);

            quadVAO_ = quadVBO_ = 0;

            meshes_.clear();
            lights_.clear();
            buffers_.clear();
            passes_.clear();
        }
        
        /**
        * @brief  It overrides the drawing of each of the 
        */
        virtual void render(const glm::mat4& view, const glm::mat4& proj) = 0;
        
        /**
        * @brief  It attaches a pass to a renderer
        * @param[in] vPath: vertex shader
        * @param[in] fPath: fragment shader   
        * @param[in] pass type: type of the pass to create   
        */

        KPass* subscribePass(const char* vPath, const char* fPath, PassType type_);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
        *         This variance creates an HDR color buffer (standard texture)
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @return KBuffer ptr
        */
        KBuffer* subscribeBuffer(int width, int height);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
        This variance creates a buffer with various textures
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return KBuffer ptr
        */
        KBuffer* subscribeBuffer(int width, int height, std::vector<KAttachment>& attachments);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
        This variance creates a buffer with various textures explicitly created through the structs
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @param[in] size: number of attachments
        * @return KBuffer ptr
        */
        KBuffer* subscribeBuffer(int width, int height, KAttachment* attachment, int size);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with a single customized attachment
                  * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments 
        * @return KBuffer ptr
        */
        KBuffer* subscribeBuffer(int width, int height, KAttachment& attachment);

        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with various textures implicitly created
                  * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return KBuffer ptr 
        */
        KBuffer* subscribeBuffer(int width, int height,  std::vector<GLAttachment>& attachments);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                 This variance creates a buffer with various textures implicitly created
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @param[in] size: number of attachments
        * @return KBuffer ptr 
        */
        KBuffer* subscribeBuffer(int width, int height, GLAttachment* attachments, int size);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with a single texture created implicitly
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return KBuffer ptr
        */
        KBuffer* subscribeBuffer(int width, int height, GLAttachment attachment);

        /**
        * @brief  It gets the las pass of a renderer
        */
        KPass* getLastPass() const;

        /**
        * @brief  It gets the last buffer of a renderer
        */
        KBuffer* getLastBuffer() const;

        /**
        * @brief  it adds a mesh to the system   
        * @return void 
        */
        void addMesh( KMesh* mesh);

        /**
        * @brief  It gets the mesh vector
        * @return mesh vector 
        */
        const std::vector<std::unique_ptr<KMesh>>& getMeshes() const;
        

        /**
        * @brief  It adds a matrix to a mesh
        * @param[in] index: the position of the mesh  
        * @param[in] matrix: the matrix to add  
        * @return void 
        */
        void addMatrixToMesh(int index, const glm::mat4& matrix);

        /**
        * @brief  it adds a mesh to the system   
        * @return void 
        */
        void addLight( KLight* light);

        /**
        * @brief  It gets the mesh vector
        * @return mesh vector 
        */
        const std::vector<std::unique_ptr<KLight>>& getLights() const;

        /**
        * @brief  It generates the quad that we will use 
        */
        void generateQuad();

        /**
        * @brief  It retrieves the quad's id 
        */
        unsigned getQuadHandle() const { return quadVAO_; }

        protected:

        int width_, height_;
        std::vector<std::unique_ptr<KBuffer>> buffers_;
        std::vector<std::unique_ptr<KPass>> passes_;
        std::vector<std::unique_ptr<KMesh>> meshes_;
        std::vector<std::unique_ptr<KLight>> lights_;

        //generic quad to render :

        unsigned quadVAO_,quadVBO_;
        // std::vector<float> quadVertices_;
    };

    class KForwardRenderer : public KRenderer
    {
        //KForwardRenderer();

        ~KForwardRenderer() override = default;

        void render(const glm::mat4& view, const glm::mat4& proj) override;
    };

    class KInstancedRenderer : public KRenderer
    {

        //KInstancedRenderer();

        ~KInstancedRenderer() override = default;

        void render(const glm::mat4& view, const glm::mat4& proj) override;
    };

    class KDeferredRenderer : public KRenderer
    {
    public: 
        KDeferredRenderer(int width, int height);

        ~KDeferredRenderer() override = default;

        void render(const glm::mat4& view, const glm::mat4& proj) override;
    };

    class KRayTraceRenderer : public KRenderer
    {
        //KRayTraceRenderer();
        ~KRayTraceRenderer() override = default;

        void render(const glm::mat4& view, const glm::mat4& proj) override;
    };
}