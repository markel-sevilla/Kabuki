#include "OpenGLTest.h"

#include "KbkTexture.h"


class KbkTextureTest : public OpenGLTest
{
protected:

    void SetUp() override
    {
        OpenGLTest::SetUp();

    }

    void TearDown() override
    {
        OpenGLTest::TearDown();
    }
};


TEST_F(KbkTextureTest, STB_Load)
{
    int width, height, channels;
    unsigned char* data = stbi_load("textures/Color.png" &width, &height, &channels, 0);

    ASSERT_NE(normalData, nullptr);

    stbi_image_free(data);
}


TEST_F(KbkTextureTest, CreatesTexture)
{
    auto tex = std::make_unique<KTexture>();
    
    int width, height, channels;

    unsigned char* data = stbi_load("textures/Color.png" &width, &height, &channels, 0);

    try
    {
        tex->generateTexture(MatType::BASE, data, width, height, channels, false);
    }
    catch(const std::exception& e)
    {
        FAIL();
    }

    SUCCEED();
}


TEST_F(KbkTextureTest, Destructor)
{
    auto tex = new KTexture();
    
    int width, height, channels;

    unsigned char* data = stbi_load("textures/Color.png" &width, &height, &channels, 0);

    tex->generateTexture(MatType::BASE, data, width, height, channels, false);

    try
    {
        delete tex;
    }
    catch(const std::exception& e)
    {
        FAIL();
    }

    SUCCEED();
}
