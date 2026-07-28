/**
 * @file    Camera.cpp
 * @brief   Simple Camera Class that gives basic notion in demos 
 *
 * @details  Its a simple Camera class that gives a bit of freedom when testing your demos
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include "glfw/glfw3.h"

#define STEP 5.F

namespace Demo
{
 
    Camera::Camera(float Fovy, float Width, float Height,
        float Near, float Far,
        glm::vec3 Position, glm::vec3 Target, glm::vec3 Up)
    {
        mView = glm::mat4(1);
        mProjection = glm::mat4(1);
        //specific variables 
        mFovy = Fovy;
        mWidth = Width;
        mHeight = Height;
        mNearPlane = Near;
        mFarPlane = Far;
        mCamPos = Position;
        mCamTarget = Target;
        mCamUp = Up;

        //spherical coordinates 
        alpha = glm::pi<float>() / 2.f;
        beta = 0;

        //create the projection
        radius = glm::length(mCamTarget - mCamPos);
        mProjection = glm::perspective(mFovy, mWidth / mHeight, mNearPlane, mFarPlane);

        glm::vec3 front = glm::normalize(mCamTarget - mCamPos);
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));

        //vectors of the camera
        mCamFront = front = glm::normalize(mCamTarget - mCamPos);
        mCamUp = glm::normalize(glm::cross(right, front));
        mView = glm::lookAt(mCamPos, mCamTarget, mCamUp);

        mCursor = glm::vec2(0);
    }

    void Camera::Update(GLFWwindow* window)
    {
        GetInput(window);

        //update vectors
        mCamTarget = mCamPos + mCamFront;

        //view matrix
        mView = glm::lookAt(mCamPos, mCamTarget, mCamUp);
    }

    void Camera::GetInput(GLFWwindow* window)
    {
        // It gets the cursor and fetchges its position at this poiunt of the frame
        double cursor_x = 0;
        double cursor_y = 0;
        glfwGetCursorPos(window, &cursor_x, &cursor_y);

        float dt = 0.016f;
        float speed = STEP;
        auto  side = glm::normalize(glm::cross(mCamFront, { 0, 1, 0 }));

        // if shift pressed then we double the speed
       if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != 0)
       {
           speed *= 2.0f;
       }

       // Move with the typical keyboard movements
       if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
       {
           mCamPos += glm::normalize(mCamFront) * speed*dt;
       }
       if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
       {
           mCamPos -= glm::normalize(mCamFront) * speed*dt;
       }
       if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
       {
           mCamPos -= glm::normalize(side) * speed*dt;
       }
       if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
       {
           mCamPos += glm::normalize(side) * speed*dt;
       }
       if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
       {
           mCamPos -= glm::vec3(0,1,0) * speed*dt;
       }
       if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
       {
           mCamPos += glm::vec3(0, 1, 0) * speed*dt;
       }

       if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
       {
           mCamFront = glm::vec3(glm::vec4(mCamFront, 0) * glm::rotate(glm::radians(15.0f) * dt, side));
       }
       if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
       {
           mCamFront = glm::vec3(glm::vec4(mCamFront, 0) * glm::rotate(-glm::radians(15.0f) * dt, side));
       }
       if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
       {
           mCamFront = glm::vec3(glm::vec4(mCamFront, 0) * glm::rotate(-glm::radians(15.0f) * dt, glm::vec3(0, 1, 0)));

       }
       if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
       {
           mCamFront = glm::vec3(glm::vec4(mCamFront, 0) * glm::rotate(glm::radians(15.0f) * dt, glm::vec3(0, 1, 0)));

       }

       if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
       {
            // View
            glm::vec2 cursor_delta = { static_cast<float>(cursor_x) - mCursor.x, static_cast<float>(cursor_y) - mCursor.y };
            mCamFront = glm::vec3(glm::vec4(mCamFront, 0) * glm::rotate(glm::radians(15.0f) * dt * cursor_delta.y, side));
            mCamFront = glm::vec3(glm::vec4(mCamFront, 0) * glm::rotate(glm::radians(15.0f) * dt * cursor_delta.x, glm::vec3(0, 1, 0)));
            mCursor = { cursor_x, cursor_y };
       }
       else
       {
           mCursor = { cursor_x, cursor_y };
       }
    }
}
