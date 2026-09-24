#ifndef HH_KBKTEXT
#define HH_KBKTEXT

#include <memory>
#include <glad/glad.h>
#include <glm/vec4.hpp>


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
    class KTexture
    {
        public:
        KTexture() = default;
        ~KTexture();
        
        /* 
        * @brief: it generates the texture with the stbi library
        * @param[in]: id - id for the txture
        * @param[in]: data - pixels
        * @param[in]: width - size of tex
        * @param[in]: height - size of tex
        * @param[in]: channels - rgba
        * @param[in]: wrap - is it wrapped?
        * @return: void  
        */
        void generateTexture(unsigned int& id, unsigned char* data, unsigned width, unsigned height, unsigned channels, bool wrap);
       
        unsigned getBaseID() const;
        unsigned getNormalID() const;
        unsigned getMetallicID() const;

        private:
        unsigned int baseID_, 
                     normalID_,
                     metallicID_;
    };

    

    //material for raytracing etc
    class KMaterial
    {
        public:
        KMaterial() = default;
        ~KMaterial() = default;

        private:
        glm::vec4 color_;
        
        bool metallic_;

        float shininess_, 
              specular_ , 
              roughness_;
    };

}

#endif