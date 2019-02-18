#include "pch.h"
#include "CApp.h"
#include "CLog.h"
#include <stdio.h>
#ifndef __EMSCRIPTEN__
#include <Windows.h>
#endif // __EMSCRIPTEN__



CApp::CApp()
{
	LayoutManager = std::unique_ptr<CLayoutManager>(new CLayoutManager);
	ModelManager = std::unique_ptr<CModelManager>(new CModelManager);
	SceneManager = std::unique_ptr<CSceneManager>(new CSceneManager);
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
		this->FrameTime = this->End - this->Start;
		//CLog::MyLog(0, "RenderTime: %d", this->FrameTime);
		this->Start = SDL_GetTicks();
		PollEvents();
		CurrentLayout = LayoutManager->GetCurrentLayout();
		CurrentScene = SceneManager->GetCurrentScene();

		ButtonList = CurrentLayout->GetObjectByType(Object2DType::OBJECT2D_BUTTON);
		for (auto o : ButtonList)
		{
			auto butt = std::dynamic_pointer_cast<CButton>(o);
			if (butt != NULL)
			{
				butt->IsClicked(Event.GetMouseData());
			}
		}

		if (MouseLock)
		{
			SDL_ShowCursor(false);
			SceneManager->GetCamera()->SetIsFree(true);
			SceneManager->GetCamera()->ProcessMouseMovements(Event.GetMouseData(),Renderer.GetWindow());
		}
		else
		{
			SceneManager->GetCamera()->SetIsFree(false);
			SDL_ShowCursor(true);
		}

		OpenGL.PreLoop();
		CurrentLayout->Tick(this->FrameTime);
		CurrentScene->Tick(this->FrameTime);

		OpenGL.UseFramebuffer("Default");
		OpenGL.PreLoopPerspective(CurrentScene->GetCamera());
		CurrentScene->Draw(&this->OpenGL);

		//OpenGL.UseFramebuffer("GUI");
		OpenGL.PreLoopOrtho(Renderer.GetWindow());
		CurrentLayout->Draw(&this->OpenGL);

		OpenGL.UseFramebuffer("0");
		OpenGL.FinalDraw();
		

		OpenGL.ProLoop(Renderer.GetWindow());
		this->End = SDL_GetTicks();

	}
}

#ifdef __EMSCRIPTEN__
void CApp::EmscriptenLoop()
{
	std::vector<std::shared_ptr<CObject2D>> ButtonList;
	std::shared_ptr<CLayout> CurrentLayout = LayoutManager->GetCurrentLayout();
	std::shared_ptr<CScene> CurrentScene = SceneManager->GetCurrentScene();
	this->FrameTime = this->End - this->Start;
	//CLog::MyLog(0, "RenderTime: %d", this->FrameTime);
	this->Start = SDL_GetTicks();
	PollEvents();
	CurrentLayout = LayoutManager->GetCurrentLayout();
	CurrentScene = SceneManager->GetCurrentScene();

	ButtonList = CurrentLayout->GetObjectByType(Object2DType::OBJECT2D_BUTTON);
	for (auto o : ButtonList)
	{
		auto butt = std::dynamic_pointer_cast<CButton>(o);
		if (butt != NULL)
		{
			butt->IsClicked(Event.GetMouseData());
		}
	}

	if (MouseLock)
	{
		SDL_ShowCursor(false);
		int w = 0; int h = 0;
		int xpos = 0; int ypos = 0;
		SDL_GetWindowPosition(Renderer.GetWindow(), &xpos, &ypos);
		SDL_GetWindowSize(Renderer.GetWindow(), &w, &h);
		this->WindowW = w;
		this->WindowH = h;
#ifndef __EMSCRIPTEN__
		SetCursorPos(xpos + (w / 2), ypos + (h / 2));
#endif // __EMSCRIPTEN__
	}
	else
	{
		SDL_ShowCursor(true);
	}

	OpenGL.PreLoop();
	//OpenGL.UseFramebuffer("GUI");
	OpenGL.UseFramebuffer("0");
	OpenGL.PreLoopOrtho(Renderer.GetWindow());

	CurrentLayout->Draw(&this->OpenGL);

	//OpenGL.UseFramebuffer("Default");
	OpenGL.PreLoopPerspective(CurrentScene->GetCamera());
	CurrentScene->Draw(&this->OpenGL);

	//OpenGL.UseFramebuffer("0");
	//OpenGL.FinalDraw();


	OpenGL.ProLoop(Renderer.GetWindow());
	this->End = SDL_GetTicks();

}
#endif // __EMSCRIPTEN__

void CApp::PollEvents()
{
	Event.PollEvents();
	this->KeyEvents(this->Event.GetKeyboardData());
	Event.GetMouseMotion(this->MouseX,this->MouseY);
	LayoutManager->SetMousePosition(MouseX, MouseY);
}

void CApp::PreLoop()
{
	OpenGL.PrepareToLoop();
	{
		OpenGL.AddNewFramebuffer("Default","Default");
		OpenGL.AddNewFramebuffer("GUI","Default");
	}
	{

		LayoutManager->AddNewLayout("Default");
		LayoutManager->ChangeCurrentLayout("Default");
		std::shared_ptr<CLayout> Layout = LayoutManager->GetLayoutByName("Default");
		Layout->SetWindowData(Renderer.GetWindow());
		Layout->AddItem(Object2DType::OBJECT2D_IMAGE, "TestImage", vec2(200.f, 100.f), vec2(100.f));
		Layout->AddItem(Object2DType::OBJECT2D_LABEL, "TestLabel", vec2(700.f, 40.f), vec2(100.f));
		//Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton", vec2(300, 300.f), vec2(128.f, 64.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton2", vec2(500.f, 300.f), vec2(100.f, 20.f));
		Layout->SetFont("Assets/Fonts/Raleway-Black.ttf");
		Layout->PrepareToLoop();

		/*CButton* TempButton = dynamic_cast<CButton*>(Layout->FindObjectByName("TestButton"));
		TempButton->LoadTexture("Assets/Textures/Tex.tga");
		TempButton->BindTexture(TempButton->GetTexture());*/
		//TempButton->AttachFunc(MouseClick);

		auto TempButton2 = std::dynamic_pointer_cast<CButton>(Layout->FindObjectByName("TestButton2"));
		TempButton2->LoadTexture("Assets/Textures/TestTex.bmp");
		TempButton2->BindTexture(TempButton2->GetTexture());
		//TempButton2->AttachFunc([]() {});
		TempButton2->Label->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 10));
		TempButton2->Label->SetText("ASDF");

		//auto TempLabel = std::dynamic_pointer_cast<CLabel>(Layout->FindObjectByName("TestLabel"));
		//TempLabel->SetText("Text Test");
	}

	//ModelManager->LoadOBJ("Assets/Models/Cube.obj","Assets/Textures/CubeBase.tga");
	ModelManager->LoadOBJ("Assets/Models/Piernik.obj", "Assets/Textures/gingerbreadhouse_tex.png");
	ModelManager->CreateMaterial("Piernik");
	ModelManager->LoadTexture("Assets/Textures/gingerbreadhouse_tex.png");
	ModelManager->LoadTexture("Assets/Textures/gingerbreadhouse_NM.png");
	ModelManager->LoadTexture("Assets/Textures/gingerbreadhouse_Spec.png");
	ModelManager->BindTextureToMaterial("Piernik", "gingerbreadhouse_tex.png", TextureTypes::BaseTex);
	ModelManager->BindTextureToMaterial("Piernik", "gingerbreadhouse_NM.png", TextureTypes::NormalMap);
	ModelManager->BindTextureToMaterial("Piernik", "gingerbreadhouse_Spec.png", TextureTypes::SpecularMap);
	ModelManager->GetModelByName("Piernik.obj")->Mat = ModelManager->GetMaterialByName("Piernik");



	SceneManager->AddNewScene("Default");
	auto tempScene(SceneManager->GetSceneByName("Default"));
	{
		tempScene->AddObjectToScene("Test");
		auto tempObject3D = tempScene->GetObjectByName("Test");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(0.f, 0.f, 5.f));
		auto tempStaticMeshComponent = dynamic_pointer_cast<CStaticMeshComponent>(tempObject3D->GetComponentByName("Mesh"));
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("Piernik.obj"));
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
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("Piernik.obj"));
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
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("Piernik.obj"));
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
		tempStaticMeshComponent->BindModel(ModelManager->GetModelByName("Piernik.obj"));
		tempStaticMeshComponent->SetPosition(vec3(0.f));
		tempStaticMeshComponent->SetRotation(vec3(0.f, 180.f, 0.f));
		tempStaticMeshComponent->AttachParrentObject(tempObject3D->GetRootComponent());
	}
	

	tempScene->AddObjectToScene("CameraTest");
	auto tempCamera = tempScene->GetObjectByName("CameraTest");
	tempCamera->AddComponent(Object3DComponent::CAMERA_COMPONENT, "Camera");
	tempCamera->SetPosition(vec3(0.f, 0.f, 0.f));
	auto Camera = dynamic_pointer_cast<CCameraComponent>(tempCamera->GetComponentByName("Camera"));

	Camera->SetFov(65.f);
	tempScene->SetCamera(Camera);

	SceneManager->SetCurrentScene("Default");



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


void CApp::KeyEvents(array<bool, 322> keys)
{
	if (keys[SDLK_1])
	{
		CLog::MyLog(0, "Pressed 1");
		this->MouseLock = !this->MouseLock;
	}
	if (keys[SDLK_w])
	{
		LayoutManager->ChangeCurrentLayout("Default");
	}
	if (keys[SDLK_s])
	{
		LayoutManager->ChangeCurrentLayout("Blank");
	}

}

