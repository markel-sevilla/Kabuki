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
#include <stb/stb_image.h>

namespace Graphics
{
    KTexture::~KTexture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);

        glDeleteTextures(1, &baseID_);
        glDeleteTextures(1, &metallicID_);
        glDeleteTextures(1, &normalID_);

    }

    void KTexture::generateTexture(unsigned int& id, unsigned char* data, unsigned width, unsigned height, unsigned channels, bool wrap)
    {

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

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
}