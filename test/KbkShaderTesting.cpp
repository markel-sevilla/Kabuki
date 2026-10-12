#include "OpenGLTest.h"

#include <filesystem>
#include <iostream>
#include "KbkShader.h"


class KbkShaderTest : public OpenGLTest
{
protected:

    void SetUp() override
    {
        OpenGLTest::SetUp();

        // TODO
    }

    void TearDown() override
    {
        // TODO

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

    ASSERT_TRUE(std::filesystem::exists(shaderPath));
}

// TEST_F(KbkShaderTest, Constructor)
// {
//     // TODO
// }


// TEST_F(KbkShaderTest, VertexShaderCompiles)
// {
//     // TODO
// }


// TEST_F(KbkShaderTest, FragmentShaderCompiles)
// {
//     // TODO
// }


// TEST_F(KbkShaderTest, ShaderProgramLinks)
// {
//     // TODO
// }


// TEST_F(KbkShaderTest, ShaderBinds)
// {
//     // TODO
// }


// TEST_F(KbkShaderTest, UniformCanBeSet)
// {
//     // TODO
// }


// TEST_F(KbkShaderTest, InvalidShaderFails)
// {
//     // TODO
// }
