#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <ctime>
#include "KbkPipeline.h"
#include "KbkCamera.h"
#include "Log.h"
#include <memory>

//main loop of the demo
int main()
{

    //init the pipeline
    auto pipeline = Graphics::KPipeline::kCreatePipelineGL(1600, 900, Graphics::KRenderType::DEF, true, true);
    
    pipeline->loadScene("data/scenes/scene.json");

    while (!glfwWindowShouldClose(pipeline->getWindow()))
    {
        glfwSwapBuffers(pipeline->getWindow());
        glfwPollEvents();

        //if we have an external camera to the pipeline for whatever reason
        //we can paste the matrices so it uses thpse
        
        pipeline->render();


        glDisable(GL_BLEND);

        glfwSwapBuffers(pipeline->getWindow());         
        glfwPollEvents();
    }

    //terminate the framework

    LOG_TRACE("Finish demo");

    return 0;
    
}