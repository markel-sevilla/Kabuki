#include "OpenGLTest.h"
#include "KbkVertex.h"

using namespace Graphics;

class KbkVertexTest : public OpenGLTest
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


TEST_F(KbkVertexTest, Constructor)
{
    // TODO
    KVertex vtx;

    EXPECT_EQ(vtx.position_, glm::vec4(0,0,0,1));
    EXPECT_EQ(vtx.normal_, glm::vec3(0,0,0));
    EXPECT_EQ(vtx.tangent_, glm::vec3(0,0,0));
    EXPECT_EQ(vtx.biTan_, glm::vec3(0,0,0));
    EXPECT_EQ(vtx.uv_, glm::vec2(0));
    EXPECT_EQ(vtx.color_, glm::vec4(0.5));
    ASSERT_EQ(vtx.avgNormal_, glm::vec3(0));
}



