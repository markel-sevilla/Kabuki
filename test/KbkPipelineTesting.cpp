#include "OpenGLTest.h"
#include "KbkPipeline.h"


class KbkPipelineTest : public OpenGLTest
{
protected:

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};


TEST_F(KbkPipelineTest, Constructor)
{
    try{
        auto pipeline = Graphics::KPipeline::kCreatePipelineGL(1600, 900, Graphics::KRenderType::DEF, true, true);
    }
    catch(const std::exception& e)
    {
        FAIL();
    }
}

TEST_F(KbkPipelineTest, CreatesBadPipelineSize)
{
    try{
        auto pipeline = Graphics::KPipeline::kCreatePipelineGL(-1, -1, static_cast<Graphics::KRenderType>(0), true, true);
    }
    catch( const std::exception& e)
    {
        SUCCEED();
        return;
    }

    FAIL();
}

TEST_F(KbkPipelineTest, CreatesBadPipelineRender)
{
     try{
        auto pipeline = Graphics::KPipeline::kCreatePipelineGL(1600, 900, static_cast<Graphics::KRenderType>(-1), true, true);
    }
    catch( const std::exception& e)
    {
        SUCCEED();
        return;
    }

    FAIL();
}