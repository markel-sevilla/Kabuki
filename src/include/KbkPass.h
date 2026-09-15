#ifndef HH_KBKPASS
#define HH_KBKPASS

#include <memory>
#include <glad/glad.h>


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
   
    class KShader;
    class KPass
    {
        public:
        /**
        * @brief  It loads the shader   
        * @param[in] shader: shader that is going to be executed
        * @return void 
        */

        KPass() = default;
        KPass(const KPass& pass) = delete;
        void operator=(const KPass& pass) = delete;
        
        KPass(KShader* shader);

        virtual void execute(GLuint buffer) = 0;

        void bindFrameBuffer(GLuint buffer);

        protected:
    
        std::unique_ptr<KShader> shader_;
    };

    class GPass : public KPass
    {

        GPass() = delete;
        GPass(const GPass& pass) = delete;
        void operator=(const GPass& pass) = delete;

        /**
        * @brief  It loads the shader  
        * @param[in] shader: shader that is going to be executed
        * @return void 
        */
        GPass(KShader* shader);

        void execute(GLuint buffer) override;

    };
}

#endif