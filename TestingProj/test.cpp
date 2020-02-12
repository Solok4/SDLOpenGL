#include "pch.h"
#include "CLog.h"
#include "CLog.cpp"

#include "CRenderer.h"
#include "CRenderer.cpp"

namespace WinTest {


	class WindowTests : public testing::Test
	{
	protected:
		void SetUp() override
		{
			TestRenderer = std::make_shared<CRenderer>();
			TestRenderer->Init();
		}

		void TearDown() override
		{
			TestRenderer->Destroy();
		}

		std::shared_ptr<CRenderer> TestRenderer;
	};


	TEST_F(WindowTests, Resize)
	{
		TestRenderer->Resize(800, 600);
		EXPECT_EQ(TestRenderer->GetWindowInfo()->ScreenWidth, 800);
		EXPECT_EQ(TestRenderer->GetWindowInfo()->ScreenHeight, 600);
	}

	TEST_F(WindowTests, ResizeDemand)
	{
		TestRenderer->GetDemandWindowInfo()->ScreenWidth = 1920;
		TestRenderer->GetDemandWindowInfo()->ScreenHeight = 1080;
		TestRenderer->ResizeDemand();
		EXPECT_EQ(TestRenderer->GetWindowInfo()->ScreenWidth, 1920);
		EXPECT_EQ(TestRenderer->GetWindowInfo()->ScreenHeight, 1080);
	}
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}