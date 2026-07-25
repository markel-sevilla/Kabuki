/**
 * @file    Camera.h
 * @brief   Simple Camera Class that gives basic notion in demos 
 *
 * @details  Its a simple Camera class that gives a bit of freedom when testing your demos
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
#pragma once
#include "glm/glm.hpp"

struct GLFWwindow;

namespace Demo
{
	struct Camera
	{

       /**
        * @brief  Construct the Camera itself
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
        
       Camera(float Fovy, float Width, float Height,
           float Near, float Far,
           glm::vec3 Position, glm::vec3 Target, glm::vec3 Up);

        
       ~Camera() = default;
       
        /**
        * @brief  Updates the matrices of the cameras 
        * @param[in] window : GLFW Window to update 
        * @return void 
        */
        void Update(GLFWwindow* window);

        /**
        * @brief  It polls the input from the glfw window  
        * @param[in] window : GLFW Window to update 
        * @return void 
        */
        void GetInput(GLFWwindow* window);
    
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


        glm::vec3 mCamPos;
        glm::vec3 mCamTarget;
        glm::vec3 mCamUp;
        glm::vec3 mCamFront;
        glm::vec2 mCursor;
	};
}