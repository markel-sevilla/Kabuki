#include <vector>

namespace Graphics{


    class KbkTextureParams{

        int t_handle_; // texture handler
        uint32_t format;
        uint32_t
    }

    class KbkDepthParams : public KbkTextureParams{

    }

    class KbKBuffer
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
        KbkBuffer(int width, int height, const std::vector<KbkTextureParams>& textures, const KbkDepthParam& depth = 0);
        KbkBuffer(int width, int height, const KbkTextureParams& textures, const KbkDepthParam& depth = 0);

        private:

        int b_handle_;
        std::vector<int> textureHandles_;
    }
}