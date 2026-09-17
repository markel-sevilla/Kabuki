#ifndef HH_KBKTEXT
#define HH_KBKTEXT

#include <memory>
#include <glad/glad.h>


/*
 * @file    KTexture.h
 * @brief   Texture and material wrapper 
 *
 * @details  encapsulates everything that a texture should have
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-09-15
 * @version 1.0
 */

namespace Graphics
{
   //vertex class
    class Vertex {

        public:
        Vertex() = default;

        //non default
        Vertex(glm::vec4 pos,  glm::vec2 uv)
        {
            for (int i = 0; i < 4; i++)
            {
                mPosition[i] = pos[i];
            }

            mUV[0] = uv[0];
            mUV[1] = uv[1];
        }

        private:
        //position, normal and texture coordinates of the vertex
        glm::vec4 mPosition = glm::vec4{ 0,0,0,1 };
        glm::vec3 mNormal = glm::vec3{ 0,0,0 };
        glm::vec3 mTangent = glm::vec3{ 0,0,0 };
        glm::vec3 mBiTan = glm::vec3{ 0,0,0 };
        glm::vec2 mUV = glm::vec2{ 0,0};
        glm::vec4 mColor = glm::vec4(0.5);
        glm::vec3 mAvgNormal = glm::vec3{ 0 };
    };

}

#endif