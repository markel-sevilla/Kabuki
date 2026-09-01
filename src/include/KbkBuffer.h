# pragma once

#include <vector>
#include "glad/glad.h"

namespace Graphics
{
    enum class GLAttachment {
        ColorHDR,       // RGBA16F - para posición, color HDR, etc.
        ColorLDR,       // RGBA8 - color normal 0-1
        ColorSingleF,   // R32F - un solo canal float (ej. luminancia, ID)
        ColorRG_F,      // RG16F - normales comprimidas, velocity buffers
        Depth,          // solo depth, sampleable
        DepthStencil,   // depth+stencil, normalmente NO sampleable (renderbuffer)
        Stencil,         // solo stencil (raro pero por completitud)
        Custom
    };

    enum class GLTarget 
    {
        Tex2D,
        Tex2DMultisample,
        Cubemap,   // requiere glFramebufferTexture2D con la cara concreta
        CubemapFace,   // requiere glFramebufferTexture2D con la cara concreta
        CubemapArray,       // completo, requiere glFramebufferTexture
        Tex2DArray,    // requiere glFramebufferTexture o glFramebufferTextureLayer
        Tex3D
    };

    struct GLFormatInfo {

        GLenum internalFormat_;
        GLenum format_;
        GLenum type_;
        GLenum attachmentPoint_; // se resuelve luego según índice de color si aplica

        bool clamped;
    };

    enum class GLAttachmentStorage
    {
        Texture,        // sampleable después
        Renderbuffer    // write-only, más rápido, no sampleable
    };

    class KAttachment
    {
        public:
        KAttachment() 
        : format_(ResolveFormat(GLAttachment::ColorHDR)), 
          target_(GLTarget::Tex2D) {
        }

        KAttachment(GLAttachment kind, GLTarget target = GLTarget::Tex2D) 
        : format_(ResolveFormat(kind)),
          target_(target)
        {}

        GLuint& getHandle() { return handle_; }
        GLFormatInfo& getFormat() { return format_; }
        GLTarget& getTarget() { return target_; }

        GLuint getHandle() const { return handle_; }
        GLFormatInfo getFormat() const { return format_; }
        GLTarget getTarget() const { return target_; }
       
        private:
        GLuint handle_; // texture handler
        GLFormatInfo format_;
        GLTarget target_;

        /**
        * @brief  It loads the format of the texture  
        * @param[in] GLAttachment kind 
        * @return void 
        */
        static GLFormatInfo ResolveFormat(GLAttachment kind);
    };

    class KBuffer
    {
        public:

        /**
        * @brief  It loads the shader from a vertex and a fragment  
        * @param[in] width: width of the buffer to paint
        * @param[in] height: height of the buffer to paint
        * @param[in] attachments parameters: color setup fpr opengl
        * @return void 
        */
        KBuffer(int width, int height, std::vector<KAttachment>& attachments);
        KBuffer(int width, int height, KAttachment* attachment, int size);
        KBuffer(int width, int height, KAttachment& attachment);
        KBuffer(int width, int height);
        

        private:

        GLuint handle_;
        uint32_t width_, height;
        GLAttachmentStorage storage_;
        std::vector<KAttachment> attachments_;
    };
}