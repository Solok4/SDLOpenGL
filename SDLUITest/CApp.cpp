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
	OpenGL.AddNewFramebuffer("Default");
	OpenGL.AddNewFramebuffer("GUI");
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
	float rot = 0;
	std::vector<std::shared_ptr<CObject2D>> ButtonList;
	std::shared_ptr<CLayout> CurrentLayout = LayoutManager->GetCurrentLayout();
	std::shared_ptr<CScene> CurrentScene = SceneManager->GetCurrentScene();
	uint32_t start = SDL_GetTicks();
	uint32_t end = SDL_GetTicks();
	while (Event.GetIsRunning())
	{
		this->FrameTime = end - start;
		//CLog::MyLog(0, "RenderTime: %d", this->FrameTime);
		start = SDL_GetTicks();
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
#ifdef __EMSCRIPTEN__
			SetCursorPos(xpos + (w / 2), ypos + (h / 2));
#endif // __EMSCRIPTEN__
		}
		else
		{
			SDL_ShowCursor(true);
		}

		OpenGL.PreLoop();
		//OpenGL.UseFramebuffer("GUI");
		OpenGL.UseFramebuffer("Default");
		OpenGL.PreLoopOrtho(Renderer.GetWindow());

	
		CurrentLayout->Draw(&this->OpenGL);
		CurrentScene->GetCamera()->SetRotation(glm::vec3(0.f, (rot + 1) / 2, 0.f));

		//OpenGL.UseFramebuffer("Default");
		OpenGL.PreLoopPerspective(CurrentScene->GetCamera());
		CurrentScene->Draw(&this->OpenGL);

		OpenGL.UseFramebuffer("0");
		OpenGL.FinalDraw();
		

		OpenGL.ProLoop(Renderer.GetWindow());
		rot++;
		end = SDL_GetTicks();

	}
}

#ifdef __EMSCRIPTEN__
void CApp::EmscriptenLoop()
{
	float rot = 0;
	std::vector<std::shared_ptr<CObject2D>> ButtonList;
	std::shared_ptr<CLayout> CurrentLayout = LayoutManager->GetCurrentLayout();
	std::shared_ptr<CScene> CurrentScene = SceneManager->GetCurrentScene();
	PollEvents();

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
		SetCursorPos(xpos + (w / 2), ypos + (h / 2));
	}
	else
	{
		SDL_ShowCursor(true);
	}
	OpenGL.PreLoop();
	OpenGL.PreLoopOrtho(Renderer.GetWindow());
	
	CurrentLayout->Draw(&this->OpenGL);
	CurrentScene->GetCamera()->SetRotation(glm::vec3(0.f, (rot + 1) / 2, 0.f));

	OpenGL.PreLoopPerspective(CurrentScene->GetCamera());
	CurrentScene->Draw(&this->OpenGL);


	OpenGL.ProLoop(Renderer.GetWindow());
	rot++;


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
		TempButton2->LoadTexture("Assets/Textures/TestTex.jpg");
		TempButton2->BindTexture(TempButton2->GetTexture());
		//TempButton2->AttachFunc([]() {});
		TempButton2->Label->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 10));
		TempButton2->Label->SetText("ASDF");

		//auto TempLabel = std::dynamic_pointer_cast<CLabel>(Layout->FindObjectByName("TestLabel"));
		//TempLabel->SetText("Text Test");
	}

	//ModelManager->LoadOBJ("Assets/Models/Cube.obj","Assets/Textures/CubeBase.tga");
	//ModelManager->LoadOBJ("Assets/Models/Piernik.obj", "Assets/Textures/PiernikTex.tga");
	ModelManager->LoadOBJ("Assets/Models/Piernik.obj", "Assets/Textures/gingerbreadhouse_tex.png");
	//std::thread t1(ModelManager, "Assets/Models/Piernik.obj", "Assets/Textures/PiernikTex.tga" );
	ModelManager->ThreadJoin();
	SceneManager->AddNewScene("Default");
	SceneManager->SetCurrentScene("Default");
	auto tempScene(SceneManager->GetSceneByName("Default"));
	{
		tempScene->AddObjectToScene("Test");
		auto tempObject3D = tempScene->GetObjectByName("Test");
		tempObject3D->AddComponent(Object3DComponent::STATIC_MESH_COMPONENT, "Mesh");
		tempObject3D->SetPosition(vec3(0.f, 0.f, 10.f));
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
		tempObject3D->SetPosition(vec3(0.f, 0.f, -10.f));
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
		tempObject3D->SetPosition(vec3(10.f, 0.f, 0.f));
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
		tempObject3D->SetPosition(vec3(-10.f, 0.f, 0.f));
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

