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
#include "KbkPass.h"
#include "Log.h"
#include "Camera.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Graphics
{

    //Helper function defined to debug shaders
    void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint /*id*/, GLenum severity, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/)
	{

		std::stringstream stream;
		stream << message << '\n';

		//take the source and check where the error is coming from
		switch (source)
		{
		case  GL_DEBUG_SOURCE_API:
			stream << "Source: API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			stream << "Source: Window Manager";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			stream << "Source: Shader Compiler";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			stream << "Source: Third Party";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			stream << "Source: Application";
			break;

		case GL_DEBUG_SOURCE_OTHER:
			stream << "Source: Other";
			break;

		default:break;
		}

		stream << '\n';

		// checkj the severity 
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			stream << "Severity: HIGH [ERROR]"; // this is an error
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			stream << "Severity: MEDIUM [WARNING]"; // warning
			break;

		case GL_DEBUG_SEVERITY_LOW:
			stream << "Severity: LOW [WARNING]"; // warning
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			stream << "Severity: NOTIFICATION [INFO]"; // just information
			return;

		default:break;
		}

		std::cerr << "OpenGL :" << type << std::endl << stream.str() << std::endl;
	}

    std::unique_ptr<KPipeline> KPipeline::kCreatePipelineGL(int width, int height, KRenderType type, bool resizeable , bool hintVisible )
    {
        return std::unique_ptr<KPipeline>(new KPipeline(width, height, KAPI::GL, type, resizeable, hintVisible));
    }
    
    std::unique_ptr<KPipeline> KPipeline::kCreatePipelineVK(int width, int height, KRenderType type, bool resizeable, bool hintVisible)
    {
        return std::unique_ptr<KPipeline>(new KPipeline(width, height, KAPI::VK, type, resizeable, hintVisible));
    }

    KPipeline::KPipeline(int width, int height, KAPI api, KRenderType type, bool resizeable, bool hintVisible) : 
                         api_(api),
                         type_(type),                     
                         size_ {width, height},
                         resizeable_(resizeable),
                         hintVisible_(hintVisible),
                         clearColor_({0,0,0,1}) 
    {
        initGLFWPipeline(width, height, resizeable, hintVisible);
        
        // Enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Optional, forces synchronous callback

		//// Set the debug message callback
		glDebugMessageCallback(GLDebugMessageCallback, NULL);

        LOG_TRACE("GLFW Initialized");

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
        LOG_TRACE("Destroy K-Pipeline");
        glfwTerminate();
    }

    void KPipeline::setClearColor(const glm::vec4& rgba)
    {
        clearColor_ = rgba;
    }
    void KPipeline::render(const glm::mat4& view, const glm::mat4& proj)
    {
        glClearColor(clearColor_.x, clearColor_.y, clearColor_.z, clearColor_.w); // keep it black so it doesn't leak into g-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // here we render with the renderer
        renderer_->render( view, proj );
    }
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    int KPipeline::initGLFWPipeline(int width, int height, bool resizeWindow, bool hintVisible)
    {
        //srand(static_cast<unsigned int>(std::time(0)));

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
        window_ = glfwCreateWindow(width, height, "K-Lib", NULL, NULL);

        if (window_ == NULL)
        {
            //In case of error
            LOG_TRACE("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }

        //create the context for the window
        glfwMakeContextCurrent(window_);
        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

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

        return 1;
    }

    void KPipeline::createForwardGL()
    {

    }
    void KPipeline::createInstancedGL()
    {

    }
    void KPipeline::createDeferredGL()
    {
        LOG_TRACE("Initialize DEFERRED OpenGL renderer");
        renderer_ = std::make_unique<KDeferredRenderer>(size_.x, size_.y);
    }
    void KPipeline::createRayTraceGL()
    {

    }

    void KPipeline::addMesh( KMesh* mesh)
    {
        renderer_->addMesh(mesh);
    }

    void KPipeline::addMeshes(const std::vector<KMesh*>& meshes)
    {
        for(auto& mesh: meshes)
            renderer_->addMesh(mesh);
    }
}