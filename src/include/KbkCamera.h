/**
 * @file    KCamera.h
 * @brief   Simple KCamera Class that gives basic notion in demos 
 *
 * @details  Its a simple KCamera class that gives a bit of freedom when testing your demos
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
#ifndef HH_KCAMERA
#define HH_KCAMERA

#include "glm/glm.hpp"
#include <memory>

struct GLFWwindow;

namespace Graphics
{
	struct KCamera
	{

       /**
        * @brief  Construct the KCamera itself
        * @param[in] Fovy field of view
        * @param[in] Width width of the frustum
        * @param[in] Height heigt of the frustum
        * @param[in] Near near plane 
        * @param[in] Far far plane 
        * @param[in] Position pos of the cam in world
        * @param[in] Target target of the cam
        * @param[in] Up up vector
        * @return none
        */
        
       KCamera(float Fovy, float Width, float Height,
           float Near, float Far,
           glm::vec3 Position, glm::vec3 Rotate);

        
       ~KCamera();
       
        /**
        * @brief  Updates the matrices of the cameras 
        * @param[in] window : GLFW Window to update 
        * @return void 
        */
        void update(GLFWwindow* window);

        /**
        * @brief  it loads the camera inside the pipeline   
        * @param[in]  scene: scene to load the cam   
        * @return void 
        */
        static std::unique_ptr<Graphics::KCamera> loadCamera(const char* filename);

        /**
        * @brief  It polls the input from the glfw window  
        * @param[in] window : GLFW Window to update 
        * @return void 
        */
        void getInput(GLFWwindow* window);
    
        /**
        * @brief  It accesses the projection matrix  
        */
        const glm::mat4& getProj() { 
            return mProjection; 
        }

        /**
        * @brief  It accesses the view matrix  
        */
        const glm::mat4& getView(){
            return mView;
        };

         /**
        * @brief  It accesses the projection matrix  
        */
        const glm::vec3& getPos() { 
            return mCamPos; 
        }

    private :
        // camera matrices
        glm::mat4 mProjection;
        glm::mat4 mView;

        // camera variables
        float     mFovy;
        float     mWidth;
        float     mHeight;
        float     mNearPlane;
        float     mFarPlane;
        
        // for spherical coordinates
        float alpha;
        float beta;
        float radius;
        float yaw_, pitch_;

        glm::vec3 mCamPos;
        glm::vec3 mCamTarget;
        glm::vec3 mCamUp;
        glm::vec3 mCamFront;
        glm::vec2 mCursor;
	};
}

#endif