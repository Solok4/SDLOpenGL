//#include "pch.h"
#include "CApp.h"
#include "CLog.h"
#include <stdio.h>
#ifndef __EMSCRIPTEN__
#include <Windows.h>
#include <io.h>
#else
#include "emscripten.h"
#endif // __EMSCRIPTEN__





bool Init()
{
	OpenGL = std::make_unique<COpengl>();
	Renderer = std::make_unique<CRenderer>();
	Event = std::make_unique < CEvent>();
	LayoutManager = std::make_unique < CLayoutManager>();
	ModelManager = std::make_unique < CModelManager>();
	SceneManager = std::make_unique < CSceneManager>();
	KeyboardConf = std::make_unique < CKeyboardConf>();
	GameplayManager = std::make_unique < CGameplayManager>();
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
		WInfo->BeginingOfTheFrame = SDL_GetTicks();
		CurrentLayout = LayoutManager->GetCurrentLayout();
		PollEvents();
		if (CurrentLayout != nullptr)
		{
			ButtonList = CurrentLayout->GetButtons();
			for (auto o : ButtonList)
			{
				auto butt = std::dynamic_pointer_cast<CButton>(o);
				if (butt != NULL)
				{
					butt->IsClicked(Event->GetMouseData());
				}
			}
		}

		CurrentScene = SceneManager->GetCurrentScene();
		if (SceneManager->GetCamera() != nullptr)
		{
			if (CurrentGameplay->GetMouseLock())
			//if (false)
			{
				SDL_ShowCursor(false);

				SceneManager->GetCamera()->SetIsFree(true);
				SceneManager->GetCamera()->ProcessMouseMovements(Event->GetMouseData(), Renderer->GetWindow());
				OpenGL->GetShadersClass().SetCurrentShaderProgram("Default");
				glUniform3f(OpenGL->GetShadersClass().GetUniformByNameStruct("Default", "CameraPos"),
					SceneManager->GetCamera()->GetPosition().x, SceneManager->GetCamera()->GetPosition().y, SceneManager->GetCamera()->GetPosition().z);
				//CLog::MyLog(LogType::Log, "CameraX: %f, CameraY: %f CameraZ: %f", SceneManager->GetCamera()->GetRotation().x, SceneManager->GetCamera()->GetRotation().y, SceneManager->GetCamera()->GetRotation().z);
			}
			else
			{
				SceneManager->GetCamera()->SetIsFree(false);
				SDL_ShowCursor(true);
			}
		}

		OpenGL->PreLoop();
		uint32_t TickTime =(WInfo->Delta * CurrentGameplay->GetTimescale());

		CurrentLayout->Tick(TickTime);
		CurrentScene->Tick(TickTime);
		LightList = CurrentScene->GetLightObjects();
		for (int i = 0; i < LightList.size() && i<MAX_LIGHTS; i++)
		{
			OpenGL->ProcessLight(LightList[i],i);
			if (LightList[i]->GetLightStruct().LightType == LightType::Point)
			{

			}
			else
			{
				CurrentScene->Draw(DrawType::VerticesOnly);
			}
			OpenGL->PostProcessLight(LightList[i], i);
		}
		OpenGL->GetShadersClass().SetCurrentShaderProgram("Default");
		glUniform1i(OpenGL->GetShadersClass().GetUniformByNameStruct("Default", "LightCount"), LightList.size());
		OpenGL->UseFramebuffer("Default");
		OpenGL->PreLoopPerspective(CurrentScene->GetCamera());
		CurrentScene->Draw(DrawType::FullDraw);

		OpenGL->UseFramebuffer("0");
		OpenGL->FinalDraw();

		OpenGL->GetShadersClass().SetCurrentShaderProgram("Gui");
		OpenGL->PreLoopOrtho(Renderer->GetWindow());
		CurrentLayout->Draw();	

		OpenGL->ProLoop(Renderer->GetWindow());

		WInfo->EndOfTheFrame = SDL_GetTicks();
		WInfo->Delta = WInfo->EndOfTheFrame - WInfo->BeginingOfTheFrame;
		if (WInfo->FPSLock != 0)
		{
			if (WInfo->Delta < (1000/ WInfo->FPSLock))
			{
				SDL_Delay((1000/ WInfo->FPSLock) - WInfo->Delta);

				WInfo->EndOfTheFrame = SDL_GetTicks();
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
	KeyEvents(KeyboardConf->GetKeyButtons());
	int MouseX = 0, MouseY = 0;
	Event->GetMouseMotion(MouseX,MouseY);
	LayoutManager->SetMousePosition(MouseX, MouseY);
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

	{

		LayoutManager->AddNewLayout("Default");
		LayoutManager->ChangeCurrentLayout("Default");
		std::shared_ptr<CLayout> Layout = LayoutManager->GetLayoutByName("Default");
		Layout->SetWindowData(Renderer->GetWindow());
		Layout->AddItem(Object2DType::OBJECT2D_IMAGE, "TestImage", vec2(200.f, 100.f), vec2(100.f));
		Layout->AddItem(Object2DType::OBJECT2D_LABEL, "TestLabel", vec2(10.f, 0.f), vec2(40.f));
		Layout->AddItem(Object2DType::OBJECT2D_LABEL, "FpsCounter", vec2(10.f, 30.f), vec2(40.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton", vec2(300.f, 300.f), vec2(128.f, 64.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton2", vec2(500.f, 300.f), vec2(100.f, 20.f));
		Layout->SetFont("Assets/Fonts/Raleway-Black.ttf");
		Layout->PrepareToLoop();

		auto TempImage = Layout->FindObjectByName<CImage>("TestImage");
		TempImage->BindTexture(ModelManager->GetImageByName("TestTex.bmp"));

		auto TempButton = Layout->FindObjectByName<CButton>("TestButton");
		TempButton->BindTexture(ModelManager->GetImageByName("Tex.tga"));
		TempButton->Label->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 10));
		TempButton->Label->SetText("First Button");
		TempButton->AttachFunc([]() {CLog::MyLog(LogType::Log, "TestButtonClick"); });

		auto TempButton2 = Layout->FindObjectByName<CButton>("TestButton2");
		TempButton2->BindTexture(ModelManager->GetImageByName("TestTex.bmp"));
		TempButton2->AttachFunc([]() {CLog::MyLog(LogType::Log, "TempButton2 Press"); });
		TempButton2->Label->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 10));
		TempButton2->Label->SetText("ASDF");

		auto TempLabel = Layout->FindObjectByName<CLabel>("TestLabel");
		TempLabel->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 16));
		TempLabel->SetText("FrameTime: ");


		auto TempLabel2 = Layout->FindObjectByName<CLabel>("FpsCounter");
		TempLabel2->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 16));
		TempLabel2->SetText("Fps: ");



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
		tempStaticMeshComponent->SetRotation(vec3(0.f, 0.f, 0.f));
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
		tempLight->SetRotation(glm::vec3(0.1f, 180.f, 0.f));
		auto light = tempLight->GetComponentByName<CLightComponent>("OrangeLight");
		light->SetLightType(LightType::Directional);
		light->SetLightBaseData(glm::vec3(0.1f), glm::vec3(0.7f), glm::vec3(0.8f));
		light->SetLightColor(vec3(1.0f,1.f,1.f));
		tempScene->AddLightToScene(tempLight);

		//auto templight2 = tempScene->AddObjectToScene("Light2", tempLight);
		//templight2->SetPosition(glm::vec3(.0f, -3.0f, 0.f));
		//auto light2 = templight2->GetComponentByName<CLightComponent>("OrangeLight");
		//light2->SetLightType(LightType::Point);
		//light2->SetLightColor(glm::vec3(0.0f, 1.0f, 0.0f));
		//light2->SetLightPointLightData(1.f, 0.09f, 0.032f);
		//tempScene->AddLightToScene(templight2);
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

	SetFPSLock(90);
	KeyboardConf->SetKeyTriggerStatus(SDL_SCANCODE_1, true);

	auto gameplay = GameplayManager->AddNewGameplay("Default");
	gameplay->SetTimescale(1.0f);
	GameplayManager->SelectCurrentGameplay("Default");
}

void SetMouseLock(bool lock)
{
	//MouseLock = lock;
}

void ResizeWindow(int w, int h)
{
	Renderer->Resize(w, h);
	LayoutManager->SetWindowData(Renderer->GetWindow());
}

void SetFPSLock(int FPS)
{
	Renderer->GetWindowInfo()->FPSLock = FPS;
}


void KeyEvents(array<bool, 322> keys)
{
	if (keys[SDL_SCANCODE_1])
	{
		CLog::MyLog(LogType::Log, "Pressed 1");
		GameplayManager->GetCurrentGameplay()->ToggleMouseLock();
		//MouseLock = !MouseLock;
	}
	if (keys[SDL_SCANCODE_T])
	{
		LayoutManager->ChangeCurrentLayout("Default");
	}
	if (keys[SDL_SCANCODE_Y])
	{
		LayoutManager->ChangeCurrentLayout("Blank");
	}
	if (keys[SDL_SCANCODE_W])
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveForward(true);
	}
	else
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveForward(false);
	}
	if (keys[SDL_SCANCODE_S])
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveBackwards(true);
	}
	else
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveBackwards(false);
	}
	if (keys[SDL_SCANCODE_A])
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveLeft(true);
	}
	else
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveLeft(false);
	}
	if (keys[SDL_SCANCODE_D])
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveRight(true);
	}
	else
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveRight(false);
	}
	if (keys[SDL_SCANCODE_SPACE])
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveUp(true);
	}
	else
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveUp(false);
	}
	if (keys[SDL_SCANCODE_LSHIFT])
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveDown(true);
	}
	else
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveDown(false);
	}
	if (keys[SDL_SCANCODE_Y])
	{
		LayoutManager->ChangeCurrentLayout("Blank");
	}

}

