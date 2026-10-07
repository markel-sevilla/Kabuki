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
   
    enum class MatType
    {
        METALLIC,
        BASE,
        NORMAL

    };

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
        void generateTexture(MatType type, unsigned char* data, unsigned width, unsigned height, unsigned channels, bool wrap);
       
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
      
        /**
         *@brief: default material constructor and destructor
         *@return: void 
         */

        KMaterial() = default;
        ~KMaterial() = default;

                /**
         * @brief Sets the material base color.
         *
         * @param color RGBA color of the material.
         */
        void setColor(const glm::vec4& color);
            
        /**
         * @brief Sets whether the material is metallic.
         *
         * @param metallic True if the material is metallic.
         */
        void setMetallic(bool metallic);
            
        /**
         * @brief Sets the material shininess.
         *
         * @param shininess Shininess coefficient of the material.
         */
        void setShininess(float shininess);
            
        /**
         * @brief Sets the material specular intensity.
         *
         * @param specular Specular intensity of the material.
         */
        void setSpecular(float specular);
            
        /**
         * @brief Sets the material roughness.
         *
         * @param roughness Roughness coefficient of the material.
         */
        void setRoughness(float roughness);
            
        /**
         * @brief Gets the material base color.
         *
         * @return The RGBA color of the material.
         */
        const glm::vec4& getColor() const;
            
        /**
         * @brief Gets whether the material is metallic.
         *
         * @return True if the material is metallic.
         */
        bool getMetallic() const;
            
        /**
         * @brief Gets the material shininess.
         *
         * @return The shininess coefficient of the material.
         */
        float getShininess() const;
            
        /**
         * @brief Gets the material specular intensity.
         *
         * @return The specular intensity of the material.
         */
        float getSpecular() const;
            
        /**
         * @brief Gets the material roughness.
         *
         * @return The roughness coefficient of the material.
         */
        float getRoughness() const;
        
        
        private:
        glm::vec4 color_;
        
        bool metallic_;

        float shininess_, 
              specular_ , 
              roughness_;
    };

}

#endif