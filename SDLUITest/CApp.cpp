#include "pch.h"
#include "CApp.h"
#include "CLog.h"
#include <stdio.h>
#ifndef __EMSCRIPTEN__
#include <Windows.h>
#endif // __EMSCRIPTEN__



CApp::CApp()
{
	this->End = 0;
	this->FPSLock = 0;
	this->FrameTime = 0;
	this->MouseX = 0;
	this->MouseY = 0;
	this->MouseLock = false;
	LayoutManager = std::unique_ptr<CLayoutManager>(new CLayoutManager);
	ModelManager = std::unique_ptr<CModelManager>(new CModelManager);
	SceneManager = std::unique_ptr<CSceneManager>(new CSceneManager);
	KeyboardConf = std::unique_ptr<CKeyboardConf>(new CKeyboardConf);
	this->SetFPSLock(0);
}


CApp::~CApp()
{
	CLog::MyLog(0, "CAppDestructor");
}



bool CApp::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		CLog::MyLog(1, "Failed to initalize SDL");
		return false;
	}
	Renderer.Init();
	OpenGL.Create(Renderer.GetWindow());
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
	PreLoop();
#ifdef __EMSCRIPTEN__
	return;
#endif // __EMSCRIPTEN__
	std::vector<std::shared_ptr<CObject2D>> ButtonList;
	std::shared_ptr<CLayout> CurrentLayout = LayoutManager->GetCurrentLayout();
	std::shared_ptr<CScene> CurrentScene = SceneManager->GetCurrentScene();
	while (Event.GetIsRunning())
	{
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
				glUniform3f(OpenGL.GetShadersClass().GetUniformByNameStruct("Default", "CameraPos"),
					SceneManager->GetCamera()->GetPosition().x, SceneManager->GetCamera()->GetPosition().y, SceneManager->GetCamera()->GetPosition().z);
				CLog::MyLog(0, "CameraX: %f, CameraY: %f CameraZ: %f", SceneManager->GetCamera()->GetPosition().x, SceneManager->GetCamera()->GetPosition().y, SceneManager->GetCamera()->GetPosition().z);
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
			if (this->FrameTime < (1000/this->FPSLock))
			{
				SDL_Delay((1000/this->FPSLock) - this->FrameTime);
			}
		}



	}
}

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
			CLog::MyLog(0, "CameraX: %f, CameraY: %f CameraZ: %f", SceneManager->GetCamera()->GetPosition().x, SceneManager->GetCamera()->GetPosition().y, SceneManager->GetCamera()->GetPosition().z);
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
	{
		OpenGL.AddNewFramebuffer("Default","Default");
	}
	{

		LayoutManager->AddNewLayout("Default");
		LayoutManager->ChangeCurrentLayout("Default");
		std::shared_ptr<CLayout> Layout = LayoutManager->GetLayoutByName("Default");
		Layout->SetWindowData(Renderer.GetWindow());
		Layout->AddItem(Object2DType::OBJECT2D_IMAGE, "TestImage", vec2(200.f, 100.f), vec2(100.f));
		Layout->AddItem(Object2DType::OBJECT2D_LABEL, "TestLabel", vec2(700.f, 40.f), vec2(100.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton", vec2(300, 300.f), vec2(128.f, 64.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton2", vec2(500.f, 300.f), vec2(100.f, 20.f));
		Layout->SetFont("Assets/Fonts/Raleway-Black.ttf");
		Layout->PrepareToLoop();

		auto TempButton = std::dynamic_pointer_cast<CButton>(Layout->FindObjectByName("TestButton"));
		TempButton->LoadTexture("Assets/Textures/Tex.tga");
		TempButton->BindTexture(TempButton->GetTexture());
		TempButton->Label->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 10));
		TempButton->Label->SetText("First Button");
		//TempButton->AttachFunc(MouseClick);

		auto TempButton2 = std::dynamic_pointer_cast<CButton>(Layout->FindObjectByName("TestButton2"));
		TempButton2->LoadTexture("Assets/Textures/TestTex.bmp");
		TempButton2->BindTexture(TempButton2->GetTexture());
		TempButton2->AttachFunc([]() {CLog::MyLog(0, "TempButton2 Press"); });
		TempButton2->Label->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 10));
		TempButton2->Label->SetText("ASDF");

		auto TempLabel = std::dynamic_pointer_cast<CLabel>(Layout->FindObjectByName("TestLabel"));
		TempLabel->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 24));
		TempLabel->SetText("Text Test");
	}

	ModelManager->LoadOBJ("Assets/Models/PiernikNorm.obj");
	ModelManager->CreateMaterial("Piernik");
	ModelManager->LoadTexture("Assets/Textures/gingerbreadhouse_tex.png");
	ModelManager->LoadTexture("Assets/Textures/gingerbreadhouse_NM.png");
	ModelManager->LoadTexture("Assets/Textures/gingerbreadhouse_Spec.png");
	ModelManager->BindTextureToMaterial("Piernik", "gingerbreadhouse_tex.png", TextureTypes::BaseTex);
	ModelManager->BindTextureToMaterial("Piernik", "gingerbreadhouse_NM.png", TextureTypes::NormalMap);
	ModelManager->BindTextureToMaterial("Piernik", "gingerbreadhouse_Spec.png", TextureTypes::SpecularMap);
	ModelManager->GetModelByName("PiernikNorm.obj")->Mat = ModelManager->GetMaterialByName("Piernik");



	SceneManager->AddNewScene("Default");
	auto tempScene(SceneManager->GetSceneByName("Default"));
	{
		tempScene->AddObjectToScene("Test");
		auto tempObject3D = tempScene->GetObjectByName("Test");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(0.f, 0.f, 5.f));
		auto tempStaticMeshComponent = dynamic_pointer_cast<CStaticMeshComponent>(tempObject3D->GetComponentByName("Mesh"));
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("PiernikNorm.obj"));
		tempStaticMeshComponent->SetPosition(vec3(0.f));
		tempStaticMeshComponent->SetRotation(vec3(0.f, 0.f, 0.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	{
		tempScene->AddObjectToScene("Test1");
		auto tempObject3D = tempScene->GetObjectByName("Test1");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(0.f, 0.f, -5.f));
		auto tempStaticMeshComponent = dynamic_pointer_cast<CStaticMeshComponent>(tempObject3D->GetComponentByName("Mesh"));
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("PiernikNorm.obj"));
		tempStaticMeshComponent->SetPosition(vec3(0.f));
		tempStaticMeshComponent->SetRotation(vec3(0.f, 180.f, 180.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	{
		tempScene->AddObjectToScene("Test2");
		auto tempObject3D = tempScene->GetObjectByName("Test2");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(5.f, 0.f, 0.f));
		auto tempStaticMeshComponent = dynamic_pointer_cast<CStaticMeshComponent>(tempObject3D->GetComponentByName("Mesh"));
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("PiernikNorm.obj"));
		tempStaticMeshComponent->SetPosition(vec3(0.f));
		tempStaticMeshComponent->SetRotation(vec3(0.f, 180.f, 90.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	{
		tempScene->AddObjectToScene("Test3");
		auto tempObject3D = tempScene->GetObjectByName("Test3");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(-5.f, 0.f, 0.f));
		auto tempStaticMeshComponent = dynamic_pointer_cast<CStaticMeshComponent>(tempObject3D->GetComponentByName("Mesh"));
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("PiernikNorm.obj"));
		tempStaticMeshComponent->SetPosition(vec3(0.f));
		tempStaticMeshComponent->SetRotation(vec3(0.f, 180.f, 0.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	{
		tempScene->AddObjectToScene("Light1");
		auto tempLight = tempScene->GetObjectByName("Light1");
		tempLight->AddComponent(Object3DComponent::LIGHT_COMPONENT, "Light");
		tempLight->SetPosition(glm::vec3(0.f, 5.0f, 0.f));
		tempLight->SetRotation(glm::vec3(0.f, 90.f, 0.f));
		auto light = dynamic_pointer_cast<CLightComponent>(tempLight->GetComponentByName("Light"));
		light->SetLightType(LightType::Directional);
		light->SetLightBaseData(glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.5f));
		light->SetLightColor(vec3(1.0f,1.f,0.f));
		tempScene->AddLightToScene(tempLight);
	}
	{
		tempScene->AddObjectToScene("Light2");
		auto tempLight = tempScene->GetObjectByName("Light2");
		tempLight->AddComponent(Object3DComponent::LIGHT_COMPONENT, "Light");
		tempLight->SetPosition(glm::vec3(0.f, 5.0f, 0.f));
		tempLight->SetRotation(glm::vec3(45.f, 0.f, 0.f));
		auto light = dynamic_pointer_cast<CLightComponent>(tempLight->GetComponentByName("Light"));
		light->SetLightType(LightType::Directional);
		light->SetLightBaseData(glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.5f));
		light->SetLightColor(vec3(0.0f, 1.f, 0.f));
		tempScene->AddLightToScene(tempLight);
	}
	

	tempScene->AddObjectToScene("CameraTest");
	auto tempCamera = tempScene->GetObjectByName("CameraTest");
	tempCamera->SetPosition(vec3(0.f, 0.f, 0.f));
	tempCamera->AddComponent(Object3DComponent::CAMERA_COMPONENT, "Camera");
	auto Camera = dynamic_pointer_cast<CCameraComponent>(tempCamera->GetComponentByName("Camera"));
	tempCamera->AddComponent(Object3DComponent::MOVEMENT_COMPONENT, "Movement");
	auto Movement = dynamic_pointer_cast<CMovementComponent>(tempCamera->GetComponentByName("Movement"));
	tempScene->SetMovementObject(Movement);

	Camera->SetFov(65.f);
	tempScene->SetCamera(Camera);

	SceneManager->SetCurrentScene("Default");

	this->SetFPSLock(90);
	this->KeyboardConf->SetKeyTriggerStatus(SDLK_1, true);
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
	if (keys[SDLK_1])
	{
		CLog::MyLog(0, "Pressed 1");
		this->MouseLock = !this->MouseLock;
	}
	if (keys[SDLK_t])
	{
		LayoutManager->ChangeCurrentLayout("Default");
	}
	if (keys[SDLK_y])
	{
		LayoutManager->ChangeCurrentLayout("Blank");
	}
	if (keys[SDLK_w])
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveForward(true);
	}
	else
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveForward(false);
	}
	if (keys[SDLK_s])
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveBackwards(true);
	}
	else
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveBackwards(false);
	}
	if (keys[SDLK_a])
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveLeft(true);
	}
	else
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveLeft(false);
	}
	if (keys[SDLK_d])
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveRight(true);
	}
	else
	{
		SceneManager->GetCurrentScene()->GetMovementObject()->MoveRight(false);
	}
	if (keys[SDLK_y])
	{
		LayoutManager->ChangeCurrentLayout("Blank");
	}

}

