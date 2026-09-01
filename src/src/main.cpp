
#include <iostream>
#include <ctime>
#include "KbkPipelineConfig.h"
#include "KbkPipeline.h"
#include "Log.h"
#include <memory>

//main loop of the demo
int main()
{

    Graphics::KPipeline* pipeline = std::unique_ptr<Graphics::KPipeline>(Graphics::KPipeline::kCreatePipelineGL(1600, 900, Graphics::KRenderType::DEF, true, true)).get();

//     renderer->ReloadShaders();
//     LOG_TRACE("Reloaded the shaders" );

//     renderer->ReloadScene();
//     LOG_TRACE("Reloaded the scene");

//     //initialize Imgui
//     renderer->ImguiInitialize(window);

//     int prevHatch = 500;
//     int bloomPasses = 10;
//     bool complexContour = false;
//     glm::vec3 prevRandCoeffs = { glm::vec3(0) };

//     //precompute the dirstortion Map
//    // renderer->mScene->ContourShaking(renderer->mContourShaking, renderer->mCam);
    
//     //precompure the hatching texture
//    // renderer->mScene->DrawHathcingTextures(renderer->mHatchingShader, renderer->mCam);
   


//     while (!glfwWindowShouldClose(window))
//     {
//         renderer->ProcessInput(window);
//         /// Start the ImGui frame
//         renderer->ImguiDisplay();

//         // 1. geometry pass: render all geometric/color data to g-buffer 
//         glClearColor(0.0, 0.0, 0.0, 1.0); // keep it black so it doesn't leak into g-buffer
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

//         //update cameras
//         renderer->mScene->Update(renderer->mCam, window);
        
//         if (renderer->mScene->mDirLight)
//         {
//             //  shadow first pass
//             renderer->mScene->ShadowPass(renderer->mShadowPassShader, renderer->mCam);
//         }
        
//         // g-buffer fill
//         renderer->mScene->GBufferPass(renderer->mGeometryShader, renderer->mCam);

//         if (renderer->mScene->showDecals)
//         {
//            //decal pass
//            renderer->mScene->DecalPass(renderer->mDecalAddShader, renderer->mCam);
//         }
       
//         if (renderer->mScene->mDepthDraw)
//         {
//             // 1.2. depth pass
//             renderer->mScene->DepthDisplay(renderer->mDepthShader, renderer->mCam);
//         }
//         else if (renderer->mScene->mDiffuseDraw)
//         {
//             // 1.3. diffuse pass
//             renderer->mScene->DiffuseDisplay(renderer->mDiffuseShader, renderer->mCam);
//         }
//         else if (renderer->mScene->mPositionsDraw)
//         {
//             // 1.4. position pass
//             renderer->mScene->PositionDisplay(renderer->mPositionShader, renderer->mCam);
//         }
//         else if (renderer->mScene->mNormalDraw)
//         {
//             // 1.5. normal pass
//             renderer->mScene->NormalDisplay(renderer->mNormalShader, renderer->mCam);
//         }
//         else
//         {

//         //    // PENCIL RENDERING SECTION
//         //     //recompute hatching based on changes if needed
//         //     if (prevHatch != renderer->mScene->hatchLines )
//         //     {
//         //         renderer->mScene->DrawHathcingTextures(renderer->mHatchingShader, renderer->mCam);
//         //         prevHatch = renderer->mScene->hatchLines;
//         //     }
           
//         //     renderer->mScene->EdgeDetection(renderer->mEdgeDetection, renderer->mCam);
            
//         //    //recompute shaking based on changes if needed
//         //    if (complexContour != renderer->mScene->complexContour || prevRandCoeffs != renderer->mScene->distortionValues)
//         //    {
//         //        prevRandCoeffs = renderer->mScene->distortionValues;
//         //        complexContour = !complexContour;
//         //        renderer->mScene->ContourShaking(renderer->mContourShaking, renderer->mCam);
//         //    }
           
//         //    //map to the distortion map
//         //    renderer->mScene->DistortionMap(renderer->mDistortionMapping, renderer->mCam);
            

//             if (renderer->mScene->ambientOclussion)
//             {
//                 // Ambient occlussionx
//                 renderer->mScene->AmbientOclussion(renderer->mAmbientOclussionShader, renderer->mCam);
            
//                 if (renderer->mScene->bilateralFilter)
//                 {
//                     //blur
//                     renderer->mScene->BilateralFilter(renderer->mBilateralFilter, renderer->mCam);
//                 }
//             }
            
//             if (!renderer->mScene->showAmbientOcclussion || !renderer->mScene->ambientOclussion)
//             {
//                 // 2. ambient pass
//                 renderer->mScene->AmbientPass(renderer->mAmbientShader, renderer->mCam);
            
//                 //3.2 lighting pass 
//                 renderer->mScene->LightingPass(renderer->mLightingShader, renderer->mCam);
            
           
//                 if (renderer->mScene->mDirLight)
//                 {
//                     //3.3 directional light
//                     renderer->mScene->DirectionalLighting(renderer->mDirLightShader, renderer->mCam);
//                 }
            
//                 if (renderer->mScene->mShowShadowMaps)
//                 {
//                     //render shadow maps
//                     renderer->mScene->DrawShadowMaps(renderer->mShadowQuad, renderer->mCam);
            
//                 }
            
//                 if (renderer->mScene->mBloom)
//                 {
//                     //4. post processing
//                     renderer->mScene->ExtractBrightness(renderer->mBrightnessExtractionShader, renderer->mCam);
//                     renderer->mScene->Bloom(renderer->mBloomShader, renderer->mCam);
//                 }
            

//             }

//             // display the edge tex 
//             renderer->mScene->Display(renderer->mDisplayShader, renderer->mCam);

//            // // connect both subsystems if needed
//             // if(renderer->mScene->internallyShade)
//             //    renderer->mScene->InternalShading(renderer->mInternalShading, renderer->mCam);

//         }

//         glDisable(GL_BLEND);

//         // Render ImGui
//         renderer->ImguiRender();

//         glfwSwapBuffers(window);
//         glfwPollEvents();

//         //take screenshot
//         if (renderer->mTakeScreenshot)
//         {
//             renderer->SaveScreenShot("screenshot.png");
//             renderer->mTakeScreenshot = false;
//         }
//     }

    //terminate the framework

    return 0;
    
}