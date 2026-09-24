#ifndef HH_KBKVERT
#define HH_KBKVERT

#include <memory>
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>


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
    class KVertex {

        public:
        KVertex() = default;

        //non default
        KVertex(glm::vec4 pos,  glm::vec2 uv)
        {
            for (int i = 0; i < 4; i++)
            {
                position_[i] = pos[i];
            }

            uv_[0] = uv[0];
            uv_[1] = uv[1];
        }

        //position, normal and texture coordinates of the vertex
        glm::vec4 position_ = glm::vec4{ 0,0,0,1 };
        glm::vec3 normal_ = glm::vec3{ 0,0,0 };
        glm::vec3 tangent_ = glm::vec3{ 0,0,0 };
        glm::vec3 biTan_ = glm::vec3{ 0,0,0 };
        glm::vec2 uv_ = glm::vec2{ 0,0};
        glm::vec4 color_ = glm::vec4(0.5);
        glm::vec3 avgNormal_ = glm::vec3{ 0 };
    };

}

#endif