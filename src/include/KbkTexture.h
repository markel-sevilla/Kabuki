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
   
   //texture
    struct KTexture
    {
        KTexture() = default;
        ~KTexture();
        
        void generateTexture(unsigned int& id, unsigned char* data, unsigned width, unsigned height, unsigned channels, bool wrap);
       
        unsigned int baseID_, 
                     normalID_,
                     metallicID_;
    };

    

    //material for raytracing etc
    struct KMaterial
    {
        KMaterial() = default;
        ~KMaterial() = default;

        glm::vec4 color_;
        
        bool metallic_;

        float shininess_, 
              specular_ , 
              roughness_;
    };

}

#endif