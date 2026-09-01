/**
 * @file    Pipeline.h
 * @brief   Simple OpenGL pipeline wrap up class 
 *
 * @details  Pipeline class that implements the abstraction for OpenGL to later be used in the stager
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
#include "KbkPipeline.h"
#include "KbkStage.h"
#include "KbkPipelineConfig.h"
#include "Log.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Graphics
{

    KPipeline* KPipeline::kCreatePipelineGL(int width, int height, KRenderType type, bool resizeable , bool hintVisible )
    {
        return new KPipeline(width, height, KAPI::GL, type, resizeable, hintVisible);
    }
    
    KPipeline* KPipeline::kCreatePipelineVK(int width, int height, KRenderType type, bool resizeable, bool hintVisible)
    {
        return new KPipeline(width, height, KAPI::VK, type, resizeable, hintVisible);
    }

    KPipeline::KPipeline(int width, int height, KAPI api, KRenderType type, bool resizeable, bool hintVisible) : 
                         api_(api),
                         type_(type),                     
                         size_ {width, height},
                         resizeable_(resizeable),
                         hintVisible_(hintVisible) 
    {
        initGLFWPipeline(width, height, resizeable, hintVisible);
        
        switch( type_ )
        {
            // we create the forward pipeline
            case KRenderType::FWD:
                createForwardGL();
                break;
            // we create a instanced pipeline
            case KRenderType::INS:
                createInstancedGL();
                break;

            // we create a deferred pipeline
            case KRenderType::DEF:
                createDeferredGL();
                break;

            // create the raytracing pipeline
            case KRenderType::RTC:
                createRayTraceGL();
                break;

            default : break;
        }
    }

    KPipeline::~KPipeline()
    {
        glfwTerminate();
    }
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    int KPipeline::initGLFWPipeline(int width, int height, bool resizeWindow, bool hintVisible)
    {
        srand(static_cast<unsigned int>(std::time(0)));

        //initialize the glfw context
        glfwInit();

       //window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, resizeWindow);
        glfwWindowHint(GLFW_VISIBLE, static_cast<int>(hintVisible));

        //Create the window
        GLFWwindow* window = glfwCreateWindow(width, height, "CS562", NULL, NULL);

        if (window == NULL)
        {
            //In case of error
            LOG_TRACE("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }

        //create the context for the window
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        //check that we have the correct glaad initialized
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_TRACE("Failed to initialize GLAD");
            return -1;
        }

        //set the viewport and the size callback
        glViewport(0, 0, width, height);
        LOG_TRACE("Reloaded the Viewport");

        glfwSwapInterval(0);

    }

    void KPipeline::createForwardGL()
    {

    }
    void KPipeline::createInstancedGL()
    {

    }
    void KPipeline::createDeferredGL()
    {
        renderer_ = std::make_unique<KDeferredRenderer>(size_.x, size_.y);
    }
    void KPipeline::createRayTraceGL()
    {

    }
}