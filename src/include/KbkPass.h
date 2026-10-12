#ifndef HH_KBKPASS
#define HH_KBKPASS

#include <memory>
#include <glad/glad.h>
#include "KbkBuffer.h"
#include "KbkShader.h"
#include "KbkMesh.h"
#include "KbkLight.h"

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
        LightingPass,
        AmbientPass,
        DisplayPass,
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
        void bindFrameBuffer(GLuint buffer, bool clear) const;

        /**
        * @brief  It copies a framebuffer to another   
        * @param[in] src: the origin fb
        * @param[in] dst: the destination fb
        * @param[in] copy: do we copy?
        * @return void 
        */
        void blitBuffers(const unsigned int src, const unsigned int dst, bool copy) const;

        
        /**
        * @brief  It renders the window as a quad   
        * @return void 
        */
        void renderQuad() const;

        /**
        * @brief  It binds a framebuffer to draw   
        * @param[in] key: the string as key
        * @param[in] buffer: the ptr to the buffer
        * @return void 
        */
        void attachBuffer(const std::string& key, KBuffer* buffer);

         /**
        * @brief  It returns the meshes  
        * @return void 
        */
        const std::vector<std::unique_ptr<KMesh>>& getMeshes() const;

        /**
        * @brief  It returns the lights  
        * @return void 
        */
        const std::vector<std::unique_ptr<KLight>>& getLights() const;

        /**
         * @brief It gets the shader
         */

        unsigned getShaderID() const;

        protected:
        
        std::unordered_map<std::string, KBuffer* > buffers_; // references to the buffers stored in the renderer 
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

    class AmbientPass : public KPass
    {

        public:
        
        AmbientPass() = delete;
        AmbientPass(const AmbientPass& pass) = delete;
        void operator=(const AmbientPass& pass) = delete;

        /**
        * @brief  It loads the shader  
        * @param[in] shader: shader that is going to be executed
        * @param[in] renderer: owner
        * @return void 
        */
        AmbientPass(KRenderer* renderer, KShader* shader);

        /**
        * @brief  It executes the pass  
        * @param[in] proj: projection matrix
        * @param[in] view: view matix 
        * @return void 
        */
        void execute(const glm::mat4& view, const glm::mat4& proj) const override;
    };

    class LightingPass : public KPass
    {

        public:
        
        LightingPass() = delete;
        LightingPass(const LightingPass& pass) = delete;
        void operator=(const LightingPass& pass) = delete;

        /**
        * @brief  It loads the shader  
        * @param[in] shader: shader that is going to be executed
        * @param[in] renderer: owner
        * @return void 
        */
        LightingPass(KRenderer* renderer, KShader* shader);

        /**
        * @brief  It executes the pass  
        * @param[in] proj: projection matrix
        * @param[in] view: view matix 
        * @return void 
        */
        void execute(const glm::mat4& view, const glm::mat4& proj) const override;
    };

    class DisplayPass : public KPass
    {

        public:
        
        DisplayPass() = delete;
        DisplayPass(const DisplayPass& pass) = delete;
        void operator=(const DisplayPass& pass) = delete;

        /**
        * @brief  It loads the shader  
        * @param[in] shader: shader that is going to be executed
        * @param[in] renderer: owner
        * @return void 
        */
        DisplayPass(KRenderer* renderer, KShader* shader);

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