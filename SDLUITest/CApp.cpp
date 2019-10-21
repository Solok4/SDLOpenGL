#include "pch.h"
#include "CApp.h"
#include "CLog.h"
#include <stdio.h>
#ifndef __EMSCRIPTEN__
#include <Windows.h>
#include <io.h>
#endif // __EMSCRIPTEN__



CApp::CApp()
{
	this->End = 0;
	this->FPSLock = 0;
	this->FrameTime = 0;
	this->MouseX = 0;
	this->MouseY = 0;
	this->MouseLock = false;
	this->LayoutManager = std::make_unique<CLayoutManager>();
	this->ModelManager = std::make_unique<CModelManager>();
	this->SceneManager = std::make_unique<CSceneManager>();
	this->KeyboardConf = std::make_unique<CKeyboardConf>();
	this->GameplayManager = std::make_unique<CGameplayManager>();
	this->SetFPSLock(0);
}


CApp::~CApp()
{
	CLog::MyLog(LogType::Log, "CAppDestructor");
	CApp::Destroy();
}



bool CApp::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		CLog::MyLog(LogType::Error, "Failed to initalize SDL");
		return false;
	}
	Renderer.Init();
	OpenGL.Create(Renderer.GetWindow());
	PreLoop();
	return true;
}

void CApp::Destroy()
{
	OpenGL.Delete();
	Renderer.Destroy();
	SDL_Quit();
}

void CApp::Loop()
{
	std::vector<std::shared_ptr<CObject2D>> ButtonList;
	std::shared_ptr<CLayout> CurrentLayout = LayoutManager->GetCurrentLayout();
	std::shared_ptr<CScene> CurrentScene = SceneManager->GetCurrentScene();
	std::shared_ptr<CGameplay> CurrentGameplay = GameplayManager->GetCurrentGameplay();
	std::vector<std::shared_ptr<CLightComponent>> LightList;
#ifndef __EMSCRIPTEN__
	while (Event.GetIsRunning())
	{
#endif 
		//CLog::MyLog(0, "RenderTime: %d", this->FrameTime);
		this->Start = SDL_GetTicks();
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
					butt->IsClicked(Event.GetMouseData());
				}
			}
		}

		CurrentScene = SceneManager->GetCurrentScene();
		if (SceneManager->GetCamera() != nullptr)
		{
			if (MouseLock)
			{
				SDL_ShowCursor(false);

				SceneManager->GetCamera()->SetIsFree(true);
				SceneManager->GetCamera()->ProcessMouseMovements(Event.GetMouseData(), Renderer.GetWindow());
				OpenGL.GetShadersClass().SetCurrentShaderProgram("Default");
				glUniform3f(OpenGL.GetShadersClass().GetUniformByNameStruct("Default", "CameraPos"),
					SceneManager->GetCamera()->GetPosition().x, SceneManager->GetCamera()->GetPosition().y, SceneManager->GetCamera()->GetPosition().z);
				//CLog::MyLog(LogType::Log, "CameraX: %f, CameraY: %f CameraZ: %f", SceneManager->GetCamera()->GetRotation().x, SceneManager->GetCamera()->GetRotation().y, SceneManager->GetCamera()->GetRotation().z);
			}
			else
			{
				SceneManager->GetCamera()->SetIsFree(false);
				SDL_ShowCursor(true);
			}
		}

		OpenGL.PreLoop();
		uint32_t TickTime = (this->FrameTime * CurrentGameplay->GetTimescale());

		CurrentLayout->Tick(TickTime);
		CurrentScene->Tick(TickTime);
		LightList = CurrentScene->GetLightObjects();
		for (int i = 0; i < LightList.size(); i++)
		{
			OpenGL.ProcessLight(LightList[i],i);
			if (LightList[i]->GetLightStruct().LightType == LightType::Point)
			{

			}
			else
			{
				CurrentScene->Draw(&this->OpenGL);
			}
			OpenGL.PostProcessLight(LightList[i], i);
		}
		OpenGL.UseFramebuffer("Default");
		OpenGL.PreLoopPerspective(CurrentScene->GetCamera());
		CurrentScene->Draw(&this->OpenGL);

		OpenGL.UseFramebuffer("0");
		OpenGL.FinalDraw();

		OpenGL.GetShadersClass().SetCurrentShaderProgram("Gui");
		OpenGL.PreLoopOrtho(Renderer.GetWindow());
		CurrentLayout->Draw(&this->OpenGL);	

		OpenGL.ProLoop(Renderer.GetWindow());

		if (this->FPSLock != 0)
		{
			if (this->FrameTime < (1000/this->FPSLock))
			{
				SDL_Delay((1000/this->FPSLock) - this->FrameTime);
			}
		}
		this->End = SDL_GetTicks();
		this->FrameTime = this->End - this->Start;
#ifndef __EMSCRIPTEN__
	}
#else
		emscripten_cancel_main_loop();
#endif
}
/*
#ifdef __EMSCRIPTEN__
void CApp::EmscriptenLoop()
{
	std::vector<std::shared_ptr<CObject2D>> ButtonList;
	std::shared_ptr<CLayout> CurrentLayout = LayoutManager->GetCurrentLayout();
	std::shared_ptr<CScene> CurrentScene = SceneManager->GetCurrentScene();
	this->Start = SDL_GetTicks();
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
				butt->IsClicked(Event.GetMouseData());
			}
		}
	}

	CurrentScene = SceneManager->GetCurrentScene();
	if (SceneManager->GetCamera() != nullptr)
	{
		if (MouseLock)
		{
			SDL_ShowCursor(false);

			SceneManager->GetCamera()->SetIsFree(true);
			SceneManager->GetCamera()->ProcessMouseMovements(Event.GetMouseData(), Renderer.GetWindow());
			glUniform3f(OpenGL.GetShadersClass().GetUniformByNameStruct("Default", "CameraPos"),
				SceneManager->GetCamera()->GetPosition().x, SceneManager->GetCamera()->GetPosition().y, SceneManager->GetCamera()->GetPosition().z);
			//CLog::MyLog(0, "CameraX: %f, CameraY: %f CameraZ: %f", SceneManager->GetCamera()->GetPosition().x, SceneManager->GetCamera()->GetPosition().y, SceneManager->GetCamera()->GetPosition().z);
		}
		else
		{
			SceneManager->GetCamera()->SetIsFree(false);
			SDL_ShowCursor(true);
		}
	}

	OpenGL.PreLoop();

	CurrentLayout->Tick(this->FrameTime);
	CurrentScene->Tick(this->FrameTime);

	OpenGL.UseFramebuffer("Default");
	OpenGL.PreLoopPerspective(CurrentScene->GetCamera());
	OpenGL.ProcessLight(CurrentScene->GetLightObjects());
	CurrentScene->Draw(&this->OpenGL);

	OpenGL.UseFramebuffer("0");
	OpenGL.FinalDraw();

	OpenGL.GetShadersClass().SetCurrentShaderProgram("Gui");
	OpenGL.PreLoopOrtho(Renderer.GetWindow());
	CurrentLayout->Draw(&this->OpenGL);


	OpenGL.ProLoop(Renderer.GetWindow());
	this->End = SDL_GetTicks();
	this->FrameTime = this->End - this->Start;



	if (this->FPSLock != 0)
	{
		if (this->FrameTime < (1000 / this->FPSLock))
		{
			SDL_Delay((1000 / this->FPSLock) - this->FrameTime);
		}
	}

}
#endif // __EMSCRIPTEN__
*/

void CApp::PollEvents()
{
	Event.PollEvents();
	this->KeyboardConf->ProcessButtons(this->Event.GetKeyboardData());
	this->KeyEvents(this->KeyboardConf->GetKeyButtons());

	//this->KeyEvents(this->Event.GetKeyboardData());
	Event.GetMouseMotion(this->MouseX,this->MouseY);
	LayoutManager->SetMousePosition(MouseX, MouseY);
}

void CApp::PreLoop()
{
	OpenGL.PrepareToLoop();
	OpenGL.AddNewFramebuffer("Default","Default");

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
		Layout->SetWindowData(Renderer.GetWindow());
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
		TempLabel->BindTickFunction([this,TempLabel](uint32_t tick) {
			std::string FrameTime = std::to_string(this->GetFrameTime());
			std::string Combined = std::string("Frametime: " + FrameTime);
			TempLabel->SetText(Combined);
			});

		auto TempLabel2 = Layout->FindObjectByName<CLabel>("FpsCounter");
		TempLabel2->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 16));
		TempLabel2->SetText("Fps: ");
		TempLabel2->BindTickFunction([this, TempLabel2](uint32_t tick) {

			uint32_t CurrentFrametime = this->GetFrameTime();
			uint32_t FrameRate = 0;
			if (CurrentFrametime > 0)
			{
				FrameRate = round(1000 / CurrentFrametime);
			}
			std::string FrameTime = std::to_string(FrameRate);
			std::string Combined = std::string("Fps: " + FrameTime);
			TempLabel2->SetText(Combined);
			});
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
		tempStaticMeshComponent->SetRotation(vec3(0.f, 180.f, 180.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	{
		auto tempObject3D = tempScene->AddObjectToScene("Test2");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(5.f, 0.f, 0.f));
		auto tempStaticMeshComponent = tempObject3D->GetComponentByName<CStaticMeshComponent>("Mesh");
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("PiernikNorm.obj"));
		tempStaticMeshComponent->SetPosition(vec3(0.f));
		tempStaticMeshComponent->SetRotation(vec3(0.f, 180.f, 90.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	{
		auto tempObject3D = tempScene->AddObjectToScene("Test3");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(-5.f, 0.f, 0.f));
		auto tempStaticMeshComponent = tempObject3D->GetComponentByName<CStaticMeshComponent>("Mesh");
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("PiernikNorm.obj"));
		tempStaticMeshComponent->SetPosition(vec3(0.f));
		tempStaticMeshComponent->SetRotation(vec3(0.f, 180.f, 0.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	{
		auto tempLight = tempScene->AddObjectToScene("Light1");
		tempLight->AddComponent(Object3DComponent::LIGHT_COMPONENT, "OrangeLight");
		tempLight->SetPosition(glm::vec3(0.f, 0.0f, 0.f));
		tempLight->SetRotation(glm::vec3(90.f, 90.f, 0.f));
		auto light = tempLight->GetComponentByName<CLightComponent>("OrangeLight");
		light->SetLightType(LightType::Directional);
		light->SetLightBaseData(glm::vec3(0.5f), glm::vec3(0.7f), glm::vec3(0.8f));
		light->SetLightColor(vec3(1.0f,1.f,1.f));
		tempScene->AddLightToScene(tempLight);

		auto templight2 = tempScene->AddObjectToScene("Light2", tempLight);
		templight2->SetPosition(glm::vec3(.0f, -3.0f, 0.f));
		auto light2 = templight2->GetComponentByName<CLightComponent>("OrangeLight");
		light2->SetLightType(LightType::Point);
		light2->SetLightColor(glm::vec3(0.0f, 1.0f, 0.0f));
		light2->SetLightPointLightData(1.f, 0.09f, 0.032f);
		tempScene->AddLightToScene(templight2);
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

	this->SetFPSLock(60);
	this->KeyboardConf->SetKeyTriggerStatus(SDL_SCANCODE_1, true);

	auto gameplay = this->GameplayManager->AddNewGameplay("Default");
	gameplay->SetTimescale(1.0f);
	this->GameplayManager->SelectCurrentGameplay("Default");
}

void CApp::SetMouseLock(bool lock)
{
	MouseLock = lock;
}

void CApp::ResizeWindow(int w, int h)
{
	Renderer.Resize(w, h);
	LayoutManager->SetWindowData(Renderer.GetWindow());
}

void CApp::SetFPSLock(int FPS)
{
	this->FPSLock = FPS;
}


void CApp::KeyEvents(array<bool, 322> keys)
{
	if (keys[SDL_SCANCODE_1])
	{
		CLog::MyLog(LogType::Log, "Pressed 1");
		this->MouseLock = !this->MouseLock;
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

