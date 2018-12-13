#include "pch.h"
#include "CApp.h"
#include "CLog.h"
#include <stdio.h>
#include <Windows.h>



CApp::CApp()
{
	std::auto_ptr<CLayoutManager> temp(new CLayoutManager);
	LayoutManager = temp;
	std::auto_ptr<CModelManager> temp2(new CModelManager);
	ModelManager = temp2;
}


CApp::~CApp()
{
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
	Renderer.OpenFont("Assets/Fonts/Raleway-Black.ttf");
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
	float rot = 0;
	std::vector<CObject2D*> ButtonList;
	std::shared_ptr<CLayout> CurrentLayout = LayoutManager->GetCurrentLayout();
	while (Event.GetIsRunning())
	{
		PollEvents();

		ButtonList = CurrentLayout->GetObjectByType(Object2DType::OBJECT2D_BUTTON);
		for (CObject2D* o : ButtonList)
		{
			CButton* butt = dynamic_cast<CButton*>(o);
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
			SetCursorPos(xpos +(w/2),ypos+(h/2));
		}
		else
		{
			SDL_ShowCursor(true);
		}

		OpenGL.PreLoop();

		OpenGL.PreLoopOrtho(Renderer.GetWindow());//Perspective?

		std::shared_ptr<CObject3D> temp(this->GetObject3DByName("Test"));
		temp->Draw(&this->OpenGL);


		OpenGL.PreLoopPerspective(); //Ortho?
		CurrentLayout->Draw(&this->OpenGL);

		OpenGL.ProLoop(Renderer.GetWindow());
		rot++;


	}
}

void CApp::PollEvents()
{
	Event.PollEvents();
	this->KeyEvents(this->Event.GetKeyboardData());
	MouseX = Event.GetMouseMotion(true);
	MouseY = Event.GetMouseMotion(false);
	LayoutManager->SetMousePosition(MouseX, MouseY);
}

void CApp::PreLoop()
{
	OpenGL.PrepareToLoop();
	{

		LayoutManager->AddNewLayout("Blank");
		LayoutManager->AddNewLayout("Default");
		LayoutManager->ChangeCurrentLayout("Default");
		std::shared_ptr<CLayout> Layout = LayoutManager->GetLayoutByName("Default");
		Layout->SetWindowData(Renderer.GetWindow());
		Layout->AddItem(Object2DType::OBJECT2D_IMAGE, "TestImage", vec2(200.f, 100.f), vec2(100.f));
		Layout->AddItem(Object2DType::OBJECT2D_LABEL, "TestLabel", vec2(200.f, 400.f), vec2(100.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton", vec2(300, 300.f), vec2(128.f, 64.f));
		Layout->AddItem(Object2DType::OBJECT2D_BUTTON, "TestButton2", vec2(500.f, 300.f), vec2(100.f, 20.f));
		Layout->SetFont("Assets/Fonts/Raleway-Black.ttf");
		Layout->PrepareToLoop();

		CButton* TempButton = dynamic_cast<CButton*>(Layout->FindObjectByName("TestButton"));
		TempButton->LoadTexture("Assets/Textures/Tex.tga");
		TempButton->BindTexture(TempButton->GetTexture());
		//TempButton->AttachFunc(MouseClick);

		CButton* TempButton2 = dynamic_cast<CButton*>(Layout->FindObjectByName("TestButton2"));
		TempButton2->LoadTexture("Assets/Textures/TestTex.jpg");
		TempButton2->BindTexture(TempButton2->GetTexture());
		//TempButton2->AttachFunc([]() {});
		TempButton2->Label->SetFont(TTF_OpenFont("Assets/Fonts/Raleway-Black.ttf", 10));
		TempButton2->Label->SetText("ASDF");

		CLabel* TempLabel = dynamic_cast<CLabel*>(Layout->FindObjectByName("TestLabel"));
		TempLabel->SetText("Text Test");
	}

	ModelManager->LoadOBJ("Assets/Models/dompiernik.obj");
	this->AddObject3D("Test");
	std::shared_ptr<CObject3D> temp(this->GetObject3DByName("Test"));
	temp->AddComponent(1, "Mesh");
	std::shared_ptr<CStaticMeshComponent> tempStaticMesh = std::dynamic_pointer_cast<CStaticMeshComponent>(temp->GetComponentByName("Mesh"));
	tempStaticMesh->BindModel(ModelManager->GetModelByName("dompiernik.obj"));

}

void CApp::SetMouseLock(bool lock)
{
	MouseLock = lock;
}

void CApp::TempLayout()
{

}

void CApp::AddObject3D(std::string name)
{
	std::shared_ptr<CObject3D> temp(new CObject3D);
	temp->SetName(name);
	Objects3D.push_back(temp);
}

std::shared_ptr<CObject3D> CApp::GetObject3DByName(std::string name)
{
	for (std::shared_ptr<CObject3D> o : Objects3D)
	{
		if (o->GetName() == name)
			return o;
	}
	return nullptr;
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
		this->LayoutManager->ChangeCurrentLayout("Default");
	}
	if (keys[SDLK_s])
	{
		this->LayoutManager->ChangeCurrentLayout("Blank");
	}

}

