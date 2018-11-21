#include "pch.h"
#include "CApp.h"
#include "CLog.h"
#include <stdio.h>
#include <Windows.h>



CApp::CApp()
{
	//label = new CLabel(1);
	//label2 = new CLabel(2);
	//butt = new CButton(3);
	Layout = new CLayout();
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
	//delete butt;
	//delete label2;
	//delete label;
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
		Layout->GetMousePosition(MouseX, MouseY);
		if (Event.GetMouseData().button ==SDL_BUTTON_LEFT && Event.GetMouseData().state == SDL_PRESSED)
		{
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
		//label2->SetPosition(vec2(1.0f, 0.0f));
		//label2->SetSize(vec2(10.0f));
		//label2->SetRotation(glm::vec3(0.0f, 0.0f, rot));
		//OpenGL.SetModelMatrix(label2->GetModelMatrix());
		//label2->Draw();

		OpenGL.PreLoopPerspective(); //Ortho?

		Layout->Draw(&this->OpenGL);

		//label->SetPosition(vec2(600.0f, 200.0f));
		//label->SetRotation(glm::vec3(180.0f, rot,0.0f));
		//label->SetObjectLayer(200);
		//OpenGL.SetModelMatrix(label->GetModelMatrix());
		//label->Draw();

		//butt->SetSize(vec2(50.0f));
		//OpenGL.SetModelMatrix(butt->GetModelMatrix());
		//butt->Draw();

		//for(CObject2D O : Objects2D)
		//{
		//	OpenGL.SetModelMatrix(O.GetModelMatrix());
		//	O.Draw();
		//}
		OpenGL.ProLoop(Renderer.GetWindow());
		rot++;


	}
}

void CApp::PollEvents()
{
	Event.PollEvents();
}

void CApp::PreLoop()
{
	OpenGL.PrepareToLoop();
	Layout->SetWindowData(Renderer.GetWindow());
	Layout->AddItem(0,vec2(200.f,100.f),vec2(100.f));
	Layout->AddItem(1, vec2(200.f, 400.f), vec2(100.f));
	Layout->SetShaderProgram(OpenGL.GetShaderProgram());
	Layout->SetFont("Assets/Fonts/Raleway-Black.ttf");
	Layout->PrepareToLoop();
	//label->Prepare();
	//label->SetFont(Renderer.GetFont());
	//label->SetText("Jula Dupa");
	////glUniform1f(glGetUniformLocation(OpenGL.GetShaderProgram(), "Tex"), 0);
	//label2->Prepare();
	//butt->Prepare();
	//butt->SetTexture("Assets/Textures/TestTex.jpg");
	//butt->AttachFunc([]() {CLog::MyLog(0, "Button Press"); });
}

void CApp::SetMouseLock(bool lock)
{
	MouseLock = lock;
}

void CApp::TempLayout()
{
	CLayout Layout;


}
