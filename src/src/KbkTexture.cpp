/**
 * @file    KTexture.h
 * @brief   wrap up for the texture 
 *
 * @details  Has basic texture info
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
#include "KbkTexture.h"
#include <stb_image.h>
#include <iostream>
#include "Log.h"

namespace Graphics
{
    KTexture::~KTexture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);

        glDeleteTextures(1, &baseID_);
        glDeleteTextures(1, &metallicID_);
        glDeleteTextures(1, &normalID_);

    }

    void KTexture::generateTexture(MatType type, unsigned char* data, unsigned width, unsigned height, unsigned channels, bool wrap)
    {

        unsigned *id = nullptr;

        switch (type)
        {
            case MatType::NORMAL:
                LOG_TRACE("Normal texture");
                id = &normalID_;
                break;
            
            case MatType::METALLIC:
                LOG_TRACE("Specular texture");
                id = &metallicID_;
                break;

            case MatType::BASE:
                LOG_TRACE("Base texture");
                id = &baseID_;
                break;
        }

        glGenTextures(1, id);
        glBindTexture(GL_TEXTURE_2D, *id);

        if (!wrap)
        {
            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else
        {
            // set the texture wrapping parameterss
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }



        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        if (channels == 2)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height,  0, GL_RG, GL_UNSIGNED_BYTE, data);
        }
        else if (channels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,  0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (channels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,  0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    unsigned KTexture::getBaseID() const
    {
        return baseID_;
    }
    unsigned KTexture::getNormalID() const
    {
        return normalID_;
    }
    unsigned KTexture::getMetallicID() const
    {
        return metallicID_;
    }

    void KMaterial::setColor(const glm::vec4& color)
    {
        color_ = color;
    }


    void KMaterial::setMetallic(bool metallic)
    {
        metallic_ = metallic;
    }


    void KMaterial::setShininess(float shininess)
    {
        shininess_ = shininess;
    }


    void KMaterial::setSpecular(float specular)
    {
        specular_ = specular;
    }


    void KMaterial::setRoughness(float roughness)
    {
        roughness_ = roughness;
    }


    const glm::vec4& KMaterial::getColor() const
    {
        return color_;
    }


    bool KMaterial::getMetallic() const
    {
        return metallic_;
    }


    float KMaterial::getShininess() const
    {
        return shininess_;
    }


    float KMaterial::getSpecular() const
    {
        return specular_;
    }


    float KMaterial::getRoughness() const
    {
        return roughness_;
    }
}