#include "OpenGLTest.h"
#include "KbkBuffer.h"


using namespace Graphics;

class KbkBufferTest : public OpenGLTest
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



TEST_F(KbkBufferTest, Constructor1)
{
    auto buffer = std::make_unique<KBuffer>(1600, 900);

    // default constructor without attachments creates an HDR color
    EXPECT_EQ(buffer->getAttachments().size(), 1);
}


TEST_F(KbkBufferTest, Constructor2)
{

    KAttachment att;
    auto buffer =  std::make_unique<KBuffer>(1600, 900, att);


    ASSERT_EQ(buffer->getAttachments().size(), 1);

    //assert that the handles are the same 
    EXPECT_EQ(buffer->getAttachmentHandle(0), att.getHandle());
}

TEST_F(KbkBufferTest, Constructor3)
{
  
    std::vector<KAttachment> att;
    att.resize(4);

    auto buffer =  std::make_unique<KBuffer>(1600, 900, att);

    //assert that the handles are the same 
    for(int i = 0; i < 4; i++)
        ASSERT_EQ(buffer->getAttachmentHandle(i), att[i].getHandle());

    EXPECT_EQ(buffer->getAttachments().size(), 4);
}

TEST_F(KbkBufferTest, Constructor4)
{
    KAttachment att[4];

    auto buffer = std::make_unique< KBuffer>(1600, 900, att, 4);

    //assert that the handles are the same 
    for(int i = 0; i < 4; i++)
        ASSERT_EQ(buffer->getAttachmentHandle(i), att[i].getHandle());

    EXPECT_EQ(buffer->getAttachments().size(), 4);
}


TEST_F(KbkBufferTest, EmptyConstructor)
{
    KAttachment att[4];

    auto buffer = std::make_unique< KBuffer>(1600, 900, att, 0);

    EXPECT_EQ(buffer->getAttachments().size(), 0);
}


TEST_F(KbkBufferTest, EmptyConstructor2)
{
    std::vector<KAttachment> att;

    auto buffer = std::make_unique<KBuffer>(1600, 900, att);

    EXPECT_EQ(buffer->getAttachments().size(), 0);
}


TEST_F(KbkBufferTest, Destructor)
{
    auto buffer = new KBuffer(1600, 900);

    try
    {
        delete buffer;
    } 
    catch (const std::exception& e)
    {
        FAIL();
    }

    SUCCEED();
}
