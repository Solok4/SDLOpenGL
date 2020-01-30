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





bool Init(int argc, char* argv)
{
	InitialSetup = std::make_unique<CInitialSetup>(argc,argv);
	OpenGL = std::make_unique<COpengl>();
	Renderer = std::make_unique<CRenderer>();
	Event = std::make_unique<CEvent>();
	LayoutManager = std::make_unique<CLayoutManager>();
	ModelManager = std::make_unique<CModelManager>();
	SceneManager = std::make_unique<CSceneManager>();
	KeyboardConf = std::make_unique<CKeyboardConf>();
	GameplayManager = std::make_unique<CGameplayManager>();
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		CLog::MyLog(LogType::Error, "Failed to initalize SDL");
		return false;
	}
	Renderer->Init();
	OpenGL->Create(Renderer->GetWindow());
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

		//Check if camera in scene manager exists
		if (SceneManager->GetCamera() != nullptr)
		{
			//If free look mode is activated
			if (CurrentGameplay->GetMouseLock())
			{
				SDL_ShowCursor(false);

				SceneManager->GetCamera()->SetIsFree(true);
			}
			else
			{
				SceneManager->GetCamera()->SetIsFree(false);
				SDL_ShowCursor(true);
			}
		}

		OpenGL->PreLoop();
		double TickTime =((WInfo->Delta.count() * CurrentGameplay->GetTimescale())*1000);
		CurrentGameplay->Tick(TickTime);
		CurrentLayout->Tick(TickTime);
		CurrentScene->Tick(TickTime);

		LightList = CurrentScene->GetLightObjects();
		int ShadowMapIterator = 0;
		int ShadowCubeIterator = 0;
		for (int i = 0; i < LightList.size() && i<MAX_LIGHTS; i++)
		{
			if (LightList[i]->IsActive())
			{
				if (LightList[i]->GetLightStruct().LightType == LightType::Point)
				{
					OpenGL->ProcessLight(LightList[i], i);
					ShadowCubeIterator++;
					CurrentScene->Draw(DrawType::VerticesOnly);
				}
				else
				{
					OpenGL->ProcessLight(LightList[i], i);
					ShadowMapIterator++;
					CurrentScene->Draw(DrawType::VerticesOnly);
				}
				OpenGL->PostProcessLight(LightList[i], i);
			}
		}
		glViewport(0, 0, Renderer->GetWindowInfo()->ScreenWidth, Renderer->GetWindowInfo()->ScreenHeight);
		OpenGL->GetShadersClass().SetCurrentShaderProgram("Default");
		glUniform1i(OpenGL->GetShadersClass().GetUniformByNameStruct("Default", "LightCount"), LightList.size());
		OpenGL->UseFramebuffer("Default");
		OpenGL->PreLoopPerspective(CurrentScene->GetCamera());
		CurrentScene->Draw(DrawType::FullDraw);
		//Lights Debug position
		OpenGL->DrawDebugLights(CurrentScene->GetLightObjects(),CurrentScene->GetCamera());
		//
		OpenGL->UseFramebuffer("0");
		OpenGL->FinalDraw();

		OpenGL->GetShadersClass().SetCurrentShaderProgram("Gui");
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
	OpenGL->AddNewFramebuffer("Default","Default");

	ModelManager->LoadOBJ("Assets/Models/PiernikNorm.obj");
	ModelManager->CreateMaterial("Piernik");
	ModelManager->LoadTexture("Assets/Textures/gingerbreadhouse_tex.png");
	ModelManager->LoadTexture("Assets/Textures/gingerbreadhouse_NM.png");
	ModelManager->LoadTexture("Assets/Textures/gingerbreadhouse_Spec.png");
	ModelManager->BindTextureToMaterial("Piernik", "gingerbreadhouse_tex.png", TextureTypes::BaseTex);
	ModelManager->BindTextureToMaterial("Piernik", "gingerbreadhouse_NM.png", TextureTypes::NormalMap);
	ModelManager->BindTextureToMaterial("Piernik", "gingerbreadhouse_Spec.png", TextureTypes::SpecularMap);
	ModelManager->GetModelByName("PiernikNorm.obj")->Mat = ModelManager->GetMaterialByName("Piernik");

	ModelManager->LoadTexture("Assets/Textures/Tex.tga");
	ModelManager->LoadTexture("Assets/Textures/TestTex.bmp");
	ModelManager->LoadOBJ("Assets/Models/Cube.obj");

	{

		auto Layout = LayoutManager->AddNewLayout("Default");
		LayoutManager->ChangeCurrentLayout("Default");
		Layout->AddItem(Object2DType::OBJECT2D_IMAGE, "TestImage", vec2(200.f, 100.f), vec2(100.f));
		Layout->AddItem(Object2DType::OBJECT2D_LABEL, "TestLabel", vec2(20.f, 20.f), vec2(40.f));
		Layout->AddItem(Object2DType::OBJECT2D_LABEL, "FpsCounter", vec2(20.f, 50.f), vec2(40.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton", vec2(300.f, 300.f), vec2(128.f, 64.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton2", vec2(500.f, 300.f), vec2(100.f, 20.f));
		Layout->PrepareToLoop();

		auto TempImage = Layout->FindObjectByName<CImage>("TestImage");
		TempImage->BindTexture(ModelManager->GetImageByName("TestTex.bmp"));

		auto TempButton = Layout->FindObjectByName<CButton>("TestButton");
		TempButton->BindTexture(ModelManager->GetImageByName("Tex.tga"));
		TempButton->GetLabel()->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 10));
		TempButton->GetLabel()->SetText("First Button");
		TempButton->AttachFunc([]() {CLog::MyLog(LogType::Log, "TestButtonClick"); });

		auto TempButton2 = Layout->FindObjectByName<CButton>("TestButton2");
		TempButton2->BindTexture(ModelManager->GetImageByName("TestTex.bmp"));
		TempButton2->AttachFunc([]() {CLog::MyLog(LogType::Log, "TempButton2 Press"); });
		TempButton2->GetLabel()->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 10));
		TempButton2->GetLabel()->SetText("ASDF");

		auto TempLabel = Layout->FindObjectByName<CLabel>("TestLabel");
		TempLabel->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 16));
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
		TempLabel2->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 16));
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

	}
	{
		auto SecondLayout = LayoutManager->AddNewLayout("DoubleTest");
		SecondLayout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButtonSecond", vec2(500.f, 500.f), vec2(100.f, 50.f));

		SecondLayout->PrepareToLoop();
		
		auto TempButton = SecondLayout->FindObjectByName<CButton>("TestButtonSecond");
		TempButton->BindTexture(ModelManager->GetImageByName("Tex.tga"));
		TempButton->GetLabel()->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 10));
		TempButton->GetLabel()->SetText("SecondLay Button");
		TempButton->AttachFunc([]() {CLog::MyLog(LogType::Log, "Second Layout Button Click"); });

		//LayoutManager->PushActiveLayout("DoubleTest");
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

	KeyboardConf->SetKeyTriggerStatus(SDL_SCANCODE_1, true);

	auto gameplay = GameplayManager->AddNewGameplay("Default");
	gameplay->SetTimescale(1.0f);
	gameplay->SetFrameLimit(80);
	GameplayManager->SelectCurrentGameplay("Default");
	
}

//void ResizeWindow(int w, int h)
//{
//	Renderer->Resize(w, h);
//	LayoutManager->RefreshWindowData();
//}
//
//void SetFPSLock(int FPS)
//{
//	Renderer->GetWindowInfo()->FPSLock = FPS;
//}