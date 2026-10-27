#include "OpenGLTest.h"

#include "KbkCamera.h"


class KbkCameraTest : public OpenGLTest
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


TEST_F(KbkCameraTest, Constructor)
{
    try{

        auto cam = std::make_unique<KCamera>(glm::radians(45), 300, 300, 0.5, 500, glm::vec3(0), glm::vec3(0));
    }
    catch (const std::exception& e)
    {
        FAIL();
    }

    EXPECT_NE(cam, nullptr);
}


TEST_F(KbkCameraTest, BadConstructor)
{
    try{

        //it has a -1, which is not allowed
        auto cam = std::make_unique<KCamera>(glm::radians(45), -1, 300, 0.5, 500, glm::vec3(0), glm::vec3(0));
    }
    catch (const std::exception& e)
    {
        SUCCEED();
        return;
    }

    FAIL();
}


TEST_F(KbkCameraTest, LoadCamera)
{

    try{
        auto cam = loadCamera("scenes/scene.json");
    }
    catch (const std::exception& e )
    {
        FAIL();
    }

    EXPECT_NE(cam, nullptr);
}


TEST_F(KbkCameraTest, Destructor)
{

    auto cam = new KCamera(glm::radians(45), 300, 300, 0.5, 500, glm::vec3(0), glm::vec3(0));

    try {
        delete cam;
    }
    catch(const std::exception& e)
    {
        FAIL();
    }
    SUCCEED();
}


// TEST_F(KbkCameraTest, ViewMatrix)
// {
//     // TODO
// }


// TEST_F(KbkCameraTest, ProjectionMatrix)
// {
//     // TODO
// }


// TEST_F(KbkCameraTest, CameraMovement)
// {
//     // TODO
// }
