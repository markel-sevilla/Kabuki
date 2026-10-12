#include "OpenGLTest.h"
#include <iostream>
#include "Log.h"

void OpenGLTest::SetUpTestSuite()
{
    ASSERT_TRUE(glfwInit());
}


void OpenGLTest::TearDownTestSuite()
{
    glfwTerminate();
}


void OpenGLTest::SetUp()
{
     //initialize the glfw context
        glfwInit();

       //window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, false);
        glfwWindowHint(GLFW_VISIBLE, static_cast<int>(true));

        //Create the window
        auto window_ = glfwCreateWindow(1600, 900, "K-Lib", NULL, NULL);

        if (window_ == NULL)
        {
            //In case of error
            LOG_TRACE("Failed to create GLFW window");
            glfwTerminate();
            FAIL();
        }

        //create the context for the window
        glfwMakeContextCurrent(window_);
       // glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

        //check that we have the correct glaad initialized
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_TRACE("Failed to initialize GLAD");
            FAIL();
        }

        //set the viewport and the size callback
        glViewport(0, 0, 1600, 900);
        LOG_TRACE("Reloaded the Viewport");

        glfwSwapInterval(0);

    const int version = gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(
            glfwGetProcAddress
        )
    );

    ASSERT_NE(version, 0);
}


void OpenGLTest::TearDown()
{
    if (window_)
    {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
}