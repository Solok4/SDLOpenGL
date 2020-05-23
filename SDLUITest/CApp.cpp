//#include "pch.h"
#include "CApp.h"
#include "CLog.h"
#include <stdio.h>
#include <chrono>
#ifndef __EMSCRIPTEN__
#include <Windows.h>
#else
#include "emscripten.h"
#endif // __EMSCRIPTEN__

bool Init(int argc, char** argv)
{
	InitialSetup = std::make_unique<CInitialSetup>(argc,argv);
	OpenGL = std::make_unique<COpengl>();
	Renderer = std::make_unique<CRenderer>();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		CLog::MyLog(LogType::Error, "Failed to initalize SDL");
		return false;
	}
	Renderer->Init();
	OpenGL->Create(Renderer->GetWindow());

	Event = std::make_unique<CEvent>();
	LayoutManager = std::make_unique<CLayoutManager>();
	ModelManager = std::make_unique<CModelManager>();
	KeyboardConf = std::make_unique<CKeyboardConf>();
	GameplayManager = std::make_unique<CGameplayManager>();
	FontManager = std::make_unique<CFontManager>();
	TextureManager = std::make_unique<CTextureManager>();
	MaterialManager = std::make_unique<CMaterialManager>();
	SceneManager = std::make_unique<CSceneManager>();
	PreLoop();
	return true;
}

void Destroy()
{
	SDL_Quit();
}

void Loop()
{
	std::vector<std::shared_ptr<CObject2D>> ButtonList;
	std::shared_ptr<CLayout> CurrentLayout = LayoutManager->GetCurrentLayout();
	std::shared_ptr<CScene> CurrentScene = SceneManager->GetCurrentScene();
	std::shared_ptr<CGameplay> CurrentGameplay = GameplayManager->GetCurrentGameplay();
	std::vector<std::shared_ptr<CLightComponent>> LightList;
	std::shared_ptr<WindowInfo> WInfo = Renderer->GetWindowInfo();
#ifndef __EMSCRIPTEN__
	while (Event->GetIsRunning())
	{
#endif 
		//CLog::MyLog(0, "RenderTime: %d", this->FrameTime);
		WInfo->BeginingOfTheFrame = std::chrono::system_clock::now();
		CurrentLayout = LayoutManager->GetCurrentLayout();
		CurrentGameplay = GameplayManager->GetCurrentGameplay();
		CurrentScene = SceneManager->GetCurrentScene();
		WInfo->FPSLock = CurrentGameplay->GetFrameLimit();

		PollEvents();

		double TickTime = ((WInfo->Delta.count() * CurrentGameplay->GetTimescale()) * 1000);
		CurrentGameplay->Tick(TickTime);
		CurrentLayout->Tick(TickTime);
		CurrentScene->Tick(TickTime);

		//Check if camera in scene manager exists
		if (CurrentScene->GetCamera() != nullptr)
		{
			//If free look mode is activated
			if (CurrentGameplay->GetMouseLock())
			{
				CurrentScene->GetCamera()->SetIsFree(true);
			}
			else
			{
				CurrentScene->GetCamera()->SetIsFree(false);
			}
		}

		if (!(CurrentGameplay->GetTimescale() == 0.0f && !CurrentGameplay->GetMouseLock()))
		{
			OpenGL->PreLoop();
			CurrentScene->Draw(DrawType::FullDraw);
			OpenGL->DrawDebugLights(CurrentScene->GetLightObjects(), CurrentScene->GetCamera());
		}
		OpenGL->FinalDraw();

		OpenGL->PreLoopOrtho();
		LayoutManager->Draw();	

		OpenGL->ProLoop(Renderer->GetWindow());

		WInfo->EndOfTheFrame = std::chrono::system_clock::now();
		WInfo->Delta = WInfo->EndOfTheFrame - WInfo->BeginingOfTheFrame;
		if (WInfo->FPSLock != 0)
		{
			if (WInfo->Delta.count()*1000 < (1000/ WInfo->FPSLock))
			{
				SDL_Delay( (uint32)round( (1000/ WInfo->FPSLock) - (WInfo->Delta.count() * 1000)));

				WInfo->EndOfTheFrame = std::chrono::system_clock::now();
				WInfo->Delta = WInfo->EndOfTheFrame - WInfo->BeginingOfTheFrame;
			}
		}
#ifndef __EMSCRIPTEN__
	}
#endif
}

void PollEvents()
{
	Event->PollEvents();
	KeyboardConf->ProcessButtons(Event->GetKeyboardData());
}

void PreLoop()
{
	OpenGL->PrepareToLoop();

	ModelManager->LoadOBJ("Assets/Models/PiernikNorm.obj");
	TextureManager->LoadTexture("Assets/Textures/gingerbreadhouse_tex.png");
	TextureManager->LoadTexture("Assets/Textures/gingerbreadhouse_NM.png");
	TextureManager->LoadTexture("Assets/Textures/gingerbreadhouse_Spec.png");

	auto PierMat = MaterialManager->CreateNewMaterial("Piernik");
	PierMat->AddTextureToMaterial("gingerbreadhouse_tex.png", TextureTypes::BaseTex);
	PierMat->AddTextureToMaterial("gingerbreadhouse_NM.png", TextureTypes::NormalMap);
	PierMat->AddTextureToMaterial("gingerbreadhouse_Spec.png", TextureTypes::SpecularMap);

	ModelManager->GetModelByName("PiernikNorm.obj")->BindMaterialToModel(PierMat);

	ModelManager->LoadOBJ("Assets/Models/Cube.obj");

	auto Font10 = FontManager->LoadFont("Assets/Fonts/Raleway-Black.ttf", 10);
	auto Font16 = FontManager->LoadFont("Assets/Fonts/Raleway-Black.ttf", 16);

	TextureManager->LoadTexture("Assets/Textures/Tex.tga");
	TextureManager->LoadTexture("Assets/Textures/TestTex.bmp");
	TextureManager->GetTextureByName("Tex.tga");

	MaterialManager->CreateNewMaterialFromFile("Assets/Models/PiernikNorm.mtl");
	{

		auto Layout = LayoutManager->AddNewLayout("Default");
		LayoutManager->ChangeCurrentLayout("Default");
		Layout->AddItem(Object2DType::OBJECT2D_IMAGE, "TestImage", vec2(200.f, 100.f), vec2(100.f));
		Layout->AddItem(Object2DType::OBJECT2D_LABEL, "FrameTimeCounter", vec2(20.f, 20.f), vec2(40.f));
		Layout->AddItem(Object2DType::OBJECT2D_LABEL, "FpsCounter", vec2(20.f, 50.f), vec2(40.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton", vec2(300.f, 300.f), vec2(128.f, 64.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton2", vec2(500.f, 300.f), vec2(100.f, 20.f));
		Layout->AddItem(Object2DType::OBJECT2D_TEXTBOX, "TestTextBox", vec2(300.f, 500.f), vec2(100.f, 20.f));
		Layout->AddItem(Object2DType::OBJECT2D_LISTBOX, "TestListBox", vec2(800.f, 200.f), vec2(300.f, 300.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "PositionTest", vec2(300.f, 600.f), vec2(100.f, 20.f));
		Layout->PrepareToLoop();

		auto TempImage = Layout->FindObjectByName<CImage>("TestImage");
		TempImage->BindTexture(TextureManager->GetTextureByName("TestTex.bmp"));

		auto TempButton = Layout->FindObjectByName<CButton>("TestButton");
		TempButton->BindTexture(TextureManager->GetTextureByName("Tex.tga"));
		TempButton->GetLabel()->SetFont(Font10);
		TempButton->GetLabel()->SetText("First Button");
		TempButton->AttachFunc([]() {CLog::MyLog(LogType::Log, "TestButtonClick"); },MouseButton::LEFTMOUSEBUTTON);

		auto TempButton2 = Layout->FindObjectByName<CButton>("TestButton2");
		TempButton2->BindTexture(TextureManager->GetTextureByName("TestTex.bmp"));
		TempButton2->AttachFunc([]() {CLog::MyLog(LogType::Log, "TempButton2 Press"); }, MouseButton::LEFTMOUSEBUTTON);
		TempButton2->GetLabel()->SetFont(Font10);
		TempButton2->GetLabel()->SetText("ASDF");

		auto TestTextBox = Layout->FindObjectByName<CTextBox>("TestTextBox");
		TestTextBox->BindTexture(TextureManager->GetTextureByName("TestTex.bmp"));
		TestTextBox->GetLabel()->SetFont(Font16);

		auto TestListBox = Layout->FindObjectByName<CListBox>("TestListBox");
		TestListBox->BindTexture(TextureManager->GetTextureByName("TestTex.bmp"));
		TestListBox->SetFont(Font16);
		TestListBox->SetText("Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book.");

		auto TempLabel = Layout->FindObjectByName<CLabel>("FrameTimeCounter");
		TempLabel->SetFont(Font16);
		TempLabel->SetText("FrameTime: ");
		TempLabel->BindTickFunction([&,TempLabel](double delta)
			{
				static int FrameCount = 0;
				FrameCount++;
				static double Time = 0;
				Time += delta;
				if (Time >= 1000)
				{
					TempLabel->SetText("FrameTime: %.2f", Time/FrameCount);
					FrameCount = 0;
					Time = 0;
				}
				
			});

		auto TempLabel2 = Layout->FindObjectByName<CLabel>("FpsCounter");
		TempLabel2->SetFont(Font16);
		TempLabel2->SetText("Fps: ");
		TempLabel2->BindTickFunction([&, TempLabel2](double delta)
			{
				static int FrameCount = 0;
				FrameCount++;
				static double Time = 0;
				Time += delta;
				if (Time >= 1000)
				{
					TempLabel2->SetText("Fps: %.2f", 1000/(Time/FrameCount));
					FrameCount = 0;
					Time = 0;
				}
				
			});

		auto PositionTest = Layout->FindObjectByName<CButton>("PositionTest");
		PositionTest->BindTexture(TextureManager->GetTextureByName("TestTex.bmp"));
		PositionTest->GetLabel()->SetFont(Font10);
		PositionTest->GetLabel()->SetText("Position Test");
		PositionTest->AttachFunc([]()
			{
				LayoutManager->ChangeCurrentLayout("Positions");
			}, MouseButton::LEFTMOUSEBUTTON);

	}
	{
		//Console
		{
			auto Console = LayoutManager->AddNewLayout("Console");
			Console->AddItem(Object2DType::OBJECT2D_LISTBOX, "Text", vec2(0, 0), vec2(Renderer->GetWindowInfo()->ScreenWidth, 450.f));
			Console->AddItem(Object2DType::OBJECT2D_TEXTBOX, "Input", vec2(0, 450), vec2(Renderer->GetWindowInfo()->ScreenWidth, 50.f));

			Console->PrepareToLoop();

			auto Listbox = Console->FindObjectByName<CListBox>("Text");
			auto Textbox = Console->FindObjectByName<CTextBox>("Input");

			Listbox->BindTexture(TextureManager->GetTextureByName("TestTex.bmp"));
			Listbox->SetFont(Font10);
			Listbox->SetText("Console Test");

			Textbox->BindTexture(TextureManager->GetTextureByName("TestTex.bmp"));
			Textbox->GetLabel()->SetFont(Font10);
			Textbox->GetLabel()->SetAligment(Object2DAligment::LEFT);
			Textbox->MoveObjectLayerUp();
		}
		//Position test layout
		{
			auto PositionTest = LayoutManager->AddNewLayout("Positions");
			PositionTest->AddItem(Object2DType::OBJECT2D_LABEL, "LeftTop", vec2(0.f, 10.f), vec2(1.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_LABEL, "Top", vec2(0.f, 10.f), vec2(1.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_LABEL, "RightTop", vec2(0.f, 10.f), vec2(1.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_LABEL, "Left", vec2(0.f), vec2(1.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_LABEL, "Center", vec2(0.f), vec2(1.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_LABEL, "Right", vec2(0.f), vec2(1.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_LABEL, "LeftBottom", vec2(0.f), vec2(1.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_LABEL, "Bottom", vec2(0.f), vec2(1.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_LABEL, "RightBottom", vec2(0.f), vec2(1.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_BUTTON, "GoBack", vec2(200.f), vec2(100.f, 50.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_BUTTON, "LeftTopButton", vec2(100.f), vec2(100.f, 50.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_BUTTON, "CenterButton", vec2(0.f), vec2(100.f, 50.f));
			PositionTest->AddItem(Object2DType::OBJECT2D_BUTTON, "RightBottomButton", vec2(0.f), vec2(100.f, 50.f));

			PositionTest->PrepareToLoop();

			auto LeftTop = PositionTest->FindObjectByName<CLabel>("LeftTop");
			LeftTop->SetFont(Font10);
			LeftTop->SetText("Left Top");
			LeftTop->SetAligment(Object2DAligment::LEFT_TOP);
			auto Top = PositionTest->FindObjectByName<CLabel>("Top");
			Top->SetFont(Font10);
			Top->SetText("Top");
			Top->SetAligment(Object2DAligment::TOP);
			auto RightTop = PositionTest->FindObjectByName<CLabel>("RightTop");
			RightTop->SetFont(Font10);
			RightTop->SetText("Right Top");
			RightTop->SetAligment(Object2DAligment::RIGHT_TOP);

			//
			auto Left = PositionTest->FindObjectByName<CLabel>("Left");
			Left->SetFont(Font10);
			Left->SetText("Left");
			Left->SetAligment(Object2DAligment::LEFT);
			auto Center = PositionTest->FindObjectByName<CLabel>("Center");
			Center->SetFont(Font10);
			Center->SetText("Center");
			Center->SetAligment(Object2DAligment::CENTER);
			auto Right = PositionTest->FindObjectByName<CLabel>("Right");
			Right->SetFont(Font10);
			Right->SetText("Right");
			Right->SetAligment(Object2DAligment::RIGHT);

			//
			auto LeftBottom = PositionTest->FindObjectByName<CLabel>("LeftBottom");
			LeftBottom->SetFont(Font10);
			LeftBottom->SetText("Left Bottom");
			LeftBottom->SetAligment(Object2DAligment::LEFT_BOTTOM);
			auto Bottom = PositionTest->FindObjectByName<CLabel>("Bottom");
			Bottom->SetFont(Font10);
			Bottom->SetText("Bottom");
			Bottom->SetAligment(Object2DAligment::BOTTOM);
			auto RightBottom = PositionTest->FindObjectByName<CLabel>("RightBottom");
			RightBottom->SetFont(Font10);
			RightBottom->SetText("Right Bottom");
			RightBottom->SetAligment(Object2DAligment::RIGHT_BOTTOM);

			//Goback
			auto GoBack = PositionTest->FindObjectByName<CButton>("GoBack");
			GoBack->BindTexture(TextureManager->GetTextureByName("TestTex.bmp"));
			GoBack->GetLabel()->SetFont(Font10);
			GoBack->GetLabel()->SetText("Go back");
			GoBack->AttachFunc([]()
				{
					LayoutManager->ChangeCurrentLayout("Default");
				}, MouseButton::LEFTMOUSEBUTTON);

			auto LeftTopButton = PositionTest->FindObjectByName<CButton>("LeftTopButton");
			LeftTopButton->SetAligment(Object2DAligment::LEFT_TOP);
			LeftTopButton->BindTexture(TextureManager->GetTextureByName("TestTex.bmp"));
			LeftTopButton->GetLabel()->SetFont(Font10);
			LeftTopButton->GetLabel()->SetText("Left top");

			auto CenterButton = PositionTest->FindObjectByName<CButton>("CenterButton");
			CenterButton->SetAligment(Object2DAligment::CENTER);
			CenterButton->BindTexture(TextureManager->GetTextureByName("TestTex.bmp"));
			CenterButton->GetLabel()->SetFont(Font10);
			CenterButton->GetLabel()->SetText("Center");

			auto RightBottomButton = PositionTest->FindObjectByName<CButton>("RightBottomButton");
			RightBottomButton->SetAligment(Object2DAligment::RIGHT_BOTTOM);
			RightBottomButton->BindTexture(TextureManager->GetTextureByName("TestTex.bmp"));
			RightBottomButton->GetLabel()->SetFont(Font10);
			RightBottomButton->GetLabel()->SetText("Right Bottom");
			
		}
		
	}
	SceneManager->AddNewScene("Default");
	auto tempScene(SceneManager->GetSceneByName("Default"));
	{
		auto tempObject3D = tempScene->AddObjectToScene("Test");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(0.f, 0.f, 5.f));
		auto tempStaticMeshComponent = tempObject3D->GetComponentByName<CStaticMeshComponent>("Mesh");
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("PiernikNorm.obj"));
		tempStaticMeshComponent->SetPosition(vec3(0.f));
		tempStaticMeshComponent->SetRotation(vec3(0.f, 45.f, 0.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	{
		auto tempObject3D = tempScene->AddObjectToScene("Test1");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(0.f, 0.f, -5.f));
		auto tempStaticMeshComponent = tempObject3D->GetComponentByName<CStaticMeshComponent>("Mesh");
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("PiernikNorm.obj"));
		tempStaticMeshComponent->SetPosition(vec3(0.f));
		tempStaticMeshComponent->SetRotation(vec3(0.f, 0.f, 0.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	{
		auto tempObject3D = tempScene->AddObjectToScene("Test2");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(5.f, -3.f, 0.f));
		auto tempStaticMeshComponent = tempObject3D->GetComponentByName<CStaticMeshComponent>("Mesh");
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("PiernikNorm.obj"));
		tempStaticMeshComponent->SetPosition(vec3(0.f));
		tempStaticMeshComponent->SetRotation(vec3(0.f, 0.f, 0.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	{
		auto tempObject3D = tempScene->AddObjectToScene("Test3");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(-5.f, 0.f, 0.f));
		auto tempStaticMeshComponent = tempObject3D->GetComponentByName<CStaticMeshComponent>("Mesh");
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("PiernikNorm.obj"));
		tempStaticMeshComponent->SetPosition(vec3(0.f));
		tempStaticMeshComponent->SetRotation(vec3(0.f, 0.f, 0.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	{
		auto tempLight = tempScene->AddObjectToScene("Light1");
		tempLight->AddComponent(Object3DComponent::LIGHT_COMPONENT, "OrangeLight");
		tempLight->SetPosition(glm::vec3(0.f, 0.0f, 0.f));
		tempLight->SetRotation(glm::vec3(10.1f, 50.f, 0.f));
		auto light = tempLight->GetComponentByName<CLightComponent>("OrangeLight");
		light->SetLightType(LightType::Directional);
		light->SetLightBaseData(glm::vec3(0.2f), glm::vec3(0.9f), glm::vec3(0.5f));
		light->SetLightColor(vec3(1.0f,1.f,1.f));
		tempScene->AddLightToScene(tempLight);

		auto tempLight2 = tempScene->AddObjectToScene("Light2");
		tempLight2->AddComponent(Object3DComponent::LIGHT_COMPONENT, "GreenLight");
		tempLight2->SetPosition(glm::vec3(0.f, 0.0f, 0.f));
		tempLight2->SetRotation(glm::vec3(10.1f, 50.f, 0.f));
		auto light2 = tempLight2->GetComponentByName<CLightComponent>("GreenLight");
		light2->SetLightType(LightType::Point);
		light2->SetLightColor(glm::vec3(0.0f, 1.0f, 0.0f));
		light2->SetLightPointLightData(1.f, 0.09f, 0.032f);
		tempScene->AddLightToScene(tempLight2);
	}

	
	auto tempCamera = tempScene->AddObjectToScene("CameraTest");
	tempCamera->SetPosition(vec3(0.f, 0.f, 0.f));
	tempCamera->AddComponent(Object3DComponent::CAMERA_COMPONENT, "Camera");
	auto Camera = tempCamera->GetComponentByName<CCameraComponent>("Camera");
	tempCamera->AddComponent(Object3DComponent::MOVEMENT_COMPONENT, "Movement");
	auto Movement = tempCamera->GetComponentByName<CMovementComponent>("Movement");
	tempScene->SetMovementObject(Movement);

	Camera->SetFov(65.f);
	tempScene->SetCamera(Camera);

	SceneManager->SetCurrentScene("Default");

	auto gameplay = GameplayManager->AddNewGameplay("Default");
	gameplay->SetTimescale(1.0f);
	gameplay->SetFrameLimit(60);
	GameplayManager->SelectCurrentGameplay("Default");
}