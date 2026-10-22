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
    unsigned char* normalData = stbi_load(path.c_str(), &width, &height, &channels, 0);
}


TEST_F(KbkTextureTest, CreatesTexture)
{
    // TODO
}


TEST_F(KbkTextureTest, LoadsTexture)
{
    // TODO
}


TEST_F(KbkTextureTest, BindsTexture)
{
    // TODO
}


TEST_F(KbkTextureTest, TextureParameters)
{
    // TODO
}


TEST_F(KbkTextureTest, ReleasesTexture)
{
    // TODO
}
