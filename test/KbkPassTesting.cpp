#include "OpenGLTest.h"

#include "KbkPass.h"
#include "KbkShader.h"
#include "KbkRenderer.h"
#include "Log.h"
#include <memory>

class KbkPassTest : public OpenGLTest
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


TEST_F(KbkPassTest, Constructor)
{
   using namespace Graphics;

   auto renderer = std::make_unique<KDeferredRenderer>(1600, 900);

   auto gShader = new KShader("data/shaders/geometryVertex.vs", "data/shaders/geometryFragment.fs");
   auto aShader = new KShader("data/shaders/ambientVertex.vs", "data/shaders/ambientFragment.fs");
   auto lShader = new KShader("data/shaders/lightVertex.vs", "data/shaders/lightFragment.fs");
   auto dShader = new KShader("data/shaders/displayVertex.vs", "data/shaders/displayFragment.fs");

   auto gPass  = std::make_unique<GPass>(renderer.get(), gShader);
   auto aPass  = std::make_unique<AmbientPass>(renderer.get(), aShader);
   auto lPass  = std::make_unique<LightingPass>(renderer.get(), lShader);
   auto dPass  = std::make_unique<DisplayPass>(renderer.get(), dShader);

   ASSERT_EQ(gPass->getShaderID(), gShader->ID);
   ASSERT_EQ(aPass->getShaderID(), aShader->ID);
   ASSERT_EQ(lPass->getShaderID(), lShader->ID);
   EXPECT_EQ(dPass->getShaderID(), dShader->ID);

}


TEST_F(KbkPassTest, CreatePass)
{
   using namespace Graphics;

   auto renderer = std::make_unique<KDeferredRenderer>(1600, 900);

   auto gPass = KPass::createPass(renderer.get(), "data/shaders/geometryVertex.vs", "data/shaders/geometryFragment.fs", PassType::GPass);
   auto lPass = KPass::createPass(renderer.get(), "data/shaders/ambientVertex.vs", "data/shaders/ambientFragment.fs", PassType::AmbientPass);
   auto aPass = KPass::createPass(renderer.get(), "data/shaders/lightVertex.vs", "data/shaders/lightFragment.fs", PassType::LightingPass);
   auto dPass = KPass::createPass(renderer.get(), "data/shaders/displayVertex.vs", "data/shaders/displayFragment.fs", PassType::DisplayPass);
   
   ASSERT_NE(gPass, nullptr);
   ASSERT_NE(aPass, nullptr);
   ASSERT_NE(lPass, nullptr);
   EXPECT_NE(dPass, nullptr);
}


TEST_F(KbkPassTest, Attachments)
{
   using namespace Graphics;

   auto renderer = std::make_unique<KDeferredRenderer>(1600, 900);
    // TODO
   auto gPass = KPass::createPass(renderer.get(), "data/shaders/geometryVertex.vs", "data/shaders/geometryFragment.fs", PassType::GPass);

   KAttachment att;
   auto buffer = new KBuffer(1600, 900, att);

   gPass->attachBuffer("Buffer", buffer);

   ASSERT_EQ(gPass->getBuffer("Buffer"), buffer);

   auto buffer2 = new KBuffer(1600, 900, att);

   gPass->attachBuffer("Buffer2", buffer2);

   EXPECT_EQ(gPass->getBuffer("Buffer2"), buffer2);

}

TEST_F(KbkPassTest, Destructor)
{
   using namespace Graphics;

   auto renderer = std::make_unique<KDeferredRenderer>(1600, 900);
    // TODO
   auto gPass = KPass::createPass(renderer.get(), "data/shaders/geometryVertex.vs", "data/shaders/geometryFragment.fs", PassType::GPass);

   KAttachment att;
   auto buffer = new KBuffer(1600, 900, att);

   gPass->attachBuffer("Buffer", buffer);

   gPass.reset();

   EXPECT_EQ(gPass, nullptr);
}