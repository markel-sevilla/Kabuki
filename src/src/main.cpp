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
    
    pipeline->loadObjects("data/scenes/scene.json");

    auto camera = Graphics::KCamera::loadCamera("data/scenes/scene.json");

    while (!glfwWindowShouldClose(pipeline->getWindow()))
    {
        glfwSwapBuffers(pipeline->getWindow());
        glfwPollEvents();

        camera->update(pipeline->getWindow());

        pipeline->render(camera->getView(), camera->getProj());


        glDisable(GL_BLEND);

        glfwSwapBuffers(pipeline->getWindow());         
        glfwPollEvents();
    }

    //terminate the framework

    LOG_TRACE("Finish demo");

    return 0;
    
}