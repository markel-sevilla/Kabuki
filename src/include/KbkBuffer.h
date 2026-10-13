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
        GLenum attachmentPoint_; 

        bool clamped;
    };

    enum class GLAttachmentStorage
    {
        Texture,        // sampleable later
        Renderbuffer    // write-only, no sampleable
    };

    // TODO; Think about a way of freely accesing each attachment without having to know the ecxact position in the buffer
    // Ideas : an enum class ? But what if a buffer uses less attachments and still want to use one that goes out of b?
    // put the attachments in a map instead of a vector and acces through a static key
    // create an enum class to use 
    // enum class GLAttachmentKey 
    // {
    //     PositionAtt = 0u,
    //     NormalAtt,
    //     DiffuseAtt,
    //     DepthAtt
    // };

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

         /**
        * @brief  It gets the handle of the attachment
        * @return GLuint
        */
        GLuint& getHandle() { return handle_; }

        /**
        * @brief  It gets the format of the attachment
        * @return GLFormatInfo
        */
        GLFormatInfo& getFormat() { return format_; }

        
        /**
        * @brief  It gets the target of the attachment
        * @return GLTarget
        */
        GLTarget& getTarget() { return target_; }

        /**
        * @brief  It gets the handle of the attachment
        * @return GLuint
        */
        GLuint getHandle() const { return handle_; }

          /**
        * @brief  It gets the format of the attachment
        * @return GLFormatInfo
        */
        GLFormatInfo getFormat() const { return format_; }

        /**
        * @brief  It gets the target of the attachment
        * @return GLTarget
        */
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
        
        /**
        * @brief  It retrieves the handle to the buffer  
        */
        ~KBuffer();
        
        /**
        * @brief  It retrieves the handle to the buffer  
        * @return handle  
        */
        GLuint getHandle() { return handle_; }

        /**
        * @brief  It retrieves the handle to the attachment  
        * @return handle  
        */
        GLuint getAttachmentHandle(unsigned i) {  return attachments_[i].getHandle(); }
        

        /**
        * @brief  It retrieves the attachments vector 
        * @return handle  
        */
        const std::vector<KAttachment>& getAttachments() { return attachments_; }
        
        private:
        
        std::vector<KAttachment> attachments_;
        void isFrameBufferComplete(GLenum status);
        GLuint handle_;
        uint32_t width_, height;
        GLAttachmentStorage storage_;
        
        //TODO : change this to a map so that 
        // we can use custom keys to retrieve each texture without knowing the pos in  the buffer
    };
}