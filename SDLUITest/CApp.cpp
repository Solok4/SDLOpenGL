#include "pch.h"
#include "CApp.h"
#include "CLog.h"
#include <stdio.h>
#include "CLabel.h"
#include <Windows.h>



CApp::CApp()
{
	label = new CLabel(1);
	label2 = new CLabel(2);
	butt = new CButton(3);
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
	return true;
}

void CApp::Destroy()
{
	label->Free();
	label2->Free();
	OpenGL.Delete();
	Renderer.Destroy();
	SDL_Quit();
}

void CApp::Loop()
{
	PreLoop();
	float rot = 0;
	while (Event.GetIsRunning())
	{
		PollEvents();
		MouseX = Event.GetMouseMotion(true);
		MouseY = Event.GetMouseMotion(false);
		if (Event.GetMouseData().button ==SDL_BUTTON_LEFT && Event.GetMouseData().state == SDL_PRESSED)
		{
			butt->IsClicked(Event.GetMouseData());
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
		//CLog::MyLog(0, "MouseX: " + std::to_string(MouseX) + " MouseY:" + std::to_string(MouseY));

		OpenGL.PreLoop();

		OpenGL.PreLoopOrtho(Renderer.GetWindow());//Perspective?
		//OpenGL.SetCameraPosition(glm::vec3(-30.0f+(rot/8), 0.0f, -50.0f));
		label2->SetPosition(vec2(1.0f, 0.0f));
		label2->SetSize(vec2(10.0f));
		label2->SetRotation(glm::vec3(0.0f, 0.0f, rot));
		label2->PreDraw();
		OpenGL.SetModelMatrix(label2->GetModelMatrix());
		label2->Draw();

		OpenGL.PreLoopPerspective(); //Ortho?

		label->SetPosition(vec2(600.0f, 200.0f));
		label->SetRotation(glm::vec3(0.0f, 0.0f, -rot));
		label->PreDraw();
		OpenGL.SetModelMatrix(label->GetModelMatrix());
		label->Draw();

		//butt->SetPosition(vec2(10.0f, 0.0f));
		butt->PreDraw();
		OpenGL.SetModelMatrix(butt->GetModelMatrix());
		butt->Draw();

		for(CObject2D O : Objects2D)
		{
			O.PreDraw();
			OpenGL.SetModelMatrix(O.GetModelMatrix());
			O.Draw();
		}
		OpenGL.ProLoop(Renderer.GetWindow());
		rot++;


	}
}

void CApp::PollEvents()
{
	Event.PollEvents();
	//MapButtons();
}

void CApp::PreLoop()
{
	OpenGL.PrepareToLoop();
	label->Prepare();
	label2->Prepare();
	butt->Prepare();
	butt->AttachFunc([]() {CLog::MyLog(0, "Button Press"); });
	for (CObject2D O : Objects2D)
	{
		O.Prepare();
	}
}

void CApp::MapButtons()
{
	int ButtonStates = Event.GetButtonStatus();
	switch (ButtonStates)
	{
		case SDLK_e:
		MouseLock = !MouseLock;
		break;
	default:
		break;
	}
}

void CApp::SetMouseLock(bool lock)
{
	MouseLock = lock;
}
