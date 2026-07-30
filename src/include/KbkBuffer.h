#include <vector>

namespace Graphics{

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

    struct GLFormatInfo {

        GLenum internalFormat_;
        GLenum format_;
        GLenum type_;
        GLenum attachmentPoint_; // se resuelve luego según índice de color si aplica
    };

    enum class KAttachmentStorage
    {
        Texture,        // sampleable después
        Renderbuffer    // write-only, más rápido, no sampleable
    }

    class KTexture{

        int tHandle_; // texture handler
        uint32_t
    }

    class KBuffer
    {
        public:

        /**
        * @brief  It loads the shader from a vertex and a fragment  
        * @param[in] width: width of the buffer to paint
        * @param[in] height: height of the buffer to paint
        * @param[in] texture parameters: color setup fpr opengl
        * @param[in] depth parameters: depth attachment for opengl 
        * @return void 
        */
        KBuffer(int width, int height, const std::vector<KTextureParams>& textures, const KDepthParam& depth = 0);
        KBuffer(int width, int height, const KTextureParams& textures, const KDepthParam& depth = 0);
        
        
        GLFormatInfo ResolveFormat(AttachmentKind kind);

        private:

        int b_handle_;
        std::vector<int> textureHandles_;
    }
}