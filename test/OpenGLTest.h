
#ifndef HH_OPENGL_TEST
#define HH_OPENGL_TEST

#include <gtest/gtest.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLTest : public ::testing::Test
{
protected:

    static void SetUpTestSuite();
    static void TearDownTestSuite();

    void SetUp() override;
    void TearDown() override;

    GLFWwindow* window_ = nullptr;
};

#endif // OPENGL_TEST_H