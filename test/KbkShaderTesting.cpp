#include "OpenGLTest.h"

#include <filesystem>
#include <iostream>
#include <exception>
#include "KbkShader.h"

class KbkShaderTest : public OpenGLTest
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

TEST(KShaderTest, LoadShader)
{
    const auto cwd = std::filesystem::current_path();

    std::cerr << "Working directory: " << cwd << '\n';

    const auto shaderPath = cwd / "data/shaders/geometryVertex.vs";

    std::cerr << "Shader path: " << shaderPath << '\n';
    std::cerr << "Exists: " << std::filesystem::exists(shaderPath) << '\n';

    EXPECT_EQ(std::filesystem::exists(shaderPath), true);
}

TEST_F(KbkShaderTest, Constructor)
{
    try
    {
        auto shader = std::make_unique<Graphics::KShader>("data/shaders/geometryVertex.vs", "data/shaders/geometryFragment.fs");
    }
    catch(std::exception& msg)
    {
        FAIL();
    }
}

// TEST_F(KbkShaderTest, ShaderBinds)
// {
//     // TODO
// }


TEST_F(KbkShaderTest, UniformCanBeSet)
{
    // TODO
}


// TEST_F(KbkShaderTest, InvalidShaderFails)
// {
//     // TODO
// }
