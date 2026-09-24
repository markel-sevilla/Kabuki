#ifndef HH_KBKPASS
#define HH_KBKPASS

#include <memory>
#include <glad/glad.h>
#include "KbkBuffer.h"
#include "KbkShader.h"
#include "KbkMesh.h"

/*
 * @file    KPass.h
 * @brief   Unitary stage for the pipeline 
 *
 * @details  one stage ususally represents one execution of a certain shader and its parameters
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */

namespace Graphics
{
   
    enum class PassType
    {
        GPass,
        ShadowPass,
        Total
    };

    class KShader;
    class KRenderer;

    class KPass
    {
        public:

        /**
        * @brief  It loads the shader   
        * @param[in] shader: shader that is going to be executed
        * @return void 
        */
        static std::unique_ptr<KPass> createPass(KRenderer* renderer, const char* vPath, const char* fPath, PassType type);
        

        KPass() = default;
        KPass(const KPass& pass) = delete;
        void operator=(const KPass& pass) = delete;

        /**
        * @brief  It loads the shader into a pass   
        * @param[in] shader: shader that is going to be executed
        * @return void 
        */

        KPass(KRenderer* renderer, KShader* shader);


        /**
        * @brief  It executes a certain pass   
        * @param[in] view: view matrix
        * @param[in] proj: projection matrix
        * @return void 
        */
        virtual void execute(const glm::mat4& view, const glm::mat4& proj) const = 0;


        /**
        * @brief  It binds a framebuffer to draw   
        * @param[in] buffer: the id of the buffer
        * @return void 
        */
        void bindFrameBuffer(GLuint buffer) const;

        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
        *         This variance creates an HDR color buffer (standard texture)
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @return void 
        */
        GLuint subscribeBuffer(int width, int height, const std::string& key);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
        This variance creates a buffer with various textures
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return GLuint 
        */
        GLuint subscribeBuffer(int width, int height, const std::string& key, std::vector<KAttachment>& attachments);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
        This variance creates a buffer with various textures explicitly created through the structs
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @param[in] size: number of attachments
        * @return GLuint 
        */
        GLuint subscribeBuffer(int width, int height,  const std::string& key, KAttachment* attachment, int size);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with a single customized attachment
                  * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments 
        * @return GLuint 
        */
        GLuint subscribeBuffer(int width, int height,  const std::string& key, KAttachment& attachment);

        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with various textures implicitly created
                  * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return GLuint 
        */
        GLuint subscribeBuffer(int width, int height, const std::string& key, std::vector<GLAttachment>& attachments);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                 This variance creates a buffer with various textures implicitly created
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @param[in] size: number of attachments
        * @return GLuint 
        */
        GLuint subscribeBuffer(int width, int height, const std::string& key, GLAttachment* attachments, int size);
        
        /**
        * @brief  Subscribes teh buffer to the pipeline and retrieves the handle of the buffer.
                  This variance creates a buffer with a single texture created implicitly
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] key: name of the buffer for retrieval
        * @param[in] attachments: pass the custom attachments themselves
        * @return GLuint 
        */
        GLuint subscribeBuffer(int width, int height, const std::string& key, GLAttachment attachment);

         /**
        * @brief  It executes the pass  
        * @return void 
        */
        const std::vector<std::unique_ptr<KMesh>>& getMeshes() const;

        protected:
        
        std::unordered_map<std::string, std::unique_ptr<KBuffer>> buffers_; 
        std::unique_ptr<KShader> shader_; 

        //reference to the renderer 
        KRenderer* owner_;

    };

    class GPass : public KPass
    {

        public:
        
        GPass() = delete;
        GPass(const GPass& pass) = delete;
        void operator=(const GPass& pass) = delete;

        /**
        * @brief  It loads the shader  
        * @param[in] shader: shader that is going to be executed
        * @param[in] renderer: owner
        * @return void 
        */
        GPass(KRenderer* renderer, KShader* shader);

        /**
        * @brief  It executes the pass  
        * @param[in] proj: projection matrix
        * @param[in] view: view matix 
        * @return void 
        */
        void execute(const glm::mat4& view, const glm::mat4& proj) const override;
    };
}

#endif