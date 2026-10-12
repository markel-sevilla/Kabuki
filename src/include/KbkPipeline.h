#pragma once 

/**
 * @file    KPipeline.h
 * @brief   Simple OpenGL pipeline wrap up class 
 *
 * @details  Pipeline class that implements the abstraction for OpenGL to later be used in the stager
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */

#include <glm/vec2.hpp>
#include <vector>
#include <unordered_map>
#include <memory>

#include "KbkPass.h"
#include "KbkCamera.h"
#include "KbkBuffer.h"
#include "KbkRenderer.h"
#include <stb_image.h>
#include <gltf/tiny_gltf.h>
#include <nlohmann/json.hpp>

class GLFWwindow;
class Camera;

namespace Graphics
{
    class KPass;

    enum class KAPI
    {
        GL,
        VK,
        TOTAL
    };

    enum class KRenderType
    {
        FWD,
        INS,
        DEF,
        RTC,
        TOTAL
    };

    class KPipeline
    {

        public:

        static std::unique_ptr<KPipeline> kCreatePipelineGL(int width, int height, KRenderType type = KRenderType::FWD , bool resizeable = true, bool hintVisible = true);
        static std::unique_ptr<KPipeline> kCreatePipelineVK(int width, int height, KRenderType type = KRenderType::FWD , bool resizeable = true, bool hintVisible = true);

        /**
        * @brief  It renders tehe current frame   
        * @param[in] width: GLFW camera 
        * @return void 
        */
        void render(const glm::mat4& view = glm::mat4(1.0), const glm::mat4& proj = glm::mat4(1.0));

        /**
        * @brief  sets the clear color   
        * @param[in] rgba: clear color for the window 
        * @return void 
        */

        void setClearColor(const glm::vec4& rgba);
        
        ~KPipeline();
        
        /**
        * @brief  returns the window   
        * @return void 
        */
        GLFWwindow* getWindow() const 
        {
            return window_;
        }

        /**
        * @brief  it adds a mesh to the system   
        * @return void 
        */
        void addMesh(KMesh* mesh);

        /**
        * @brief  it adds a collection of meshes to the system   
        * @return void 
        */
        void addMeshes(const std::vector<KMesh*>& mesh);

        /**
        * @brief  it loads a scene froma json   
        * @param[in]  scene: json   
        * @return void 
        */
        void loadObjects(const char* filename);

        /**
        * @brief  it loads a mesh from a gltf   
        * @param[in]  m2w: model to world of the mesh   
        * @param[in]  filename: gltf path   
        * @return void 
        */
        void loadGltf(const glm::mat4& m2w, const std::string& filename, KMesh* lightmesh = nullptr);

        /**
        * @brief  it loads a collection of nodes from a gltf   
        * @param[in]  m2w: model to world of the mesh   
        * @param[in]  filename: gltf path   
        * @param[in]  node: gltf node   
        * @param[in]  model: gltf model   
        * @return void 
        */
        void loadNodes(const glm::mat4& m2w ,tinygltf::Node& node, tinygltf::Model& model, KMesh* lightmesh, const std::string & filepath);


        /**
        * @brief  it loads the meshes inside the pipeline   
        * @param[in]  scene: scene to load the cam   
        * @return void 
        */
        void loadMeshes(const nlohmann::json& scene);

        /**
        * @brief  it loads the lights inside the pipeline   
        * @param[in]  scene: scene to load the cam   
        * @return void 
        */
        void loadLights(const nlohmann::json& scene);

        /**
        * @brief  it loads the decals inside the pipeline   
        * @param[in]  scene: scene to load the cam   
        * @return void 
        */
        void loadDecals(const nlohmann::json& scene);

        private:

        /**
        * @brief  It constructs the pipeline itself  
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] resizeWindow: window resize yes or no
        * @param[in] hintVisible: hint visible yes or no
        * @return void 
        */
        
        KPipeline( int width, int height, KAPI api = KAPI::GL, KRenderType type = KRenderType::FWD , bool resizeable = true, bool hintVisible = true);
        
        /**
        * @brief  It loads the glfw pipeline with glad  
        * @param[in] width: width of the window
        * @param[in] height: height of the window
        * @param[in] resizeWindow: window resize yes or no
        * @param[in] hintVisible: hint visible yes or no
        * @return void 
        */

        int initGLFWPipeline(int width, int height, bool resizeWindow, bool hintVisible);

        /**
        * @brief  It creates the forward gl pipeline  
        * @return void 
        */

        void createForwardGL();

        /**
        * @brief  It creates the instanced gl pipeline  
        * @return void 
        */
        void createInstancedGL();

        /**
        * @brief  It creates the deferred gl pipeline  
        * @return void 
        */
        void createDeferredGL();

        /**
        * @brief  It creates the ray tracer gl pipeline  
        * @return void 
        */
        void createRayTraceGL();

        std::unique_ptr<KRenderer> renderer_;
        glm::ivec2 size_;
        KAPI api_;
        KRenderType type_;
        GLFWwindow* window_; 
        bool resizeable_;
        bool hintVisible_;
        glm::vec4 clearColor_;
        
    };
}