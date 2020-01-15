#include "pch.h"
#include "CLog.h"
#include "CRenderer.h"


std::unique_ptr<CRenderer> Renderer;

CRenderer::CRenderer()
{
}


CRenderer::~CRenderer()
{
	CLog::MyLog(LogType::Log, "RendererDestructor");
}

void CRenderer::Init()
{
	this->WInfo = std::make_shared<WindowInfo>();
	this->WInfo->ScreenHeight = this->ScreenHeight;
	this->WInfo->ScreenWidth = this->ScreenWidth;
	this->WInfo->ScreenAspectRatio = (float)this->ScreenWidth / this->ScreenHeight;
	this->WInfo->WindowFlags = this->Flags;
	this->WInfo->BeginingOfTheFrame = std::chrono::system_clock::now();
	this->WInfo->EndOfTheFrame = std::chrono::system_clock::now();
	this->WInfo->Delta = this->WInfo->EndOfTheFrame- this->WInfo->BeginingOfTheFrame;
	this->WInfo->FPSLock = 60;

	Window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->WInfo->ScreenWidth, this->WInfo->ScreenHeight, Flags);
	if (Window == nullptr)
	{
		CLog::MyLog(LogType::Error, "Failed to create window: %s",SDL_GetError());

	}
	if (TTF_Init() != 0)
	{
		CLog::MyLog(LogType::Error, "Failed to initalize TTF: %s",TTF_GetError());
	}

	SDL_GetWindowPosition(Window, &this->WInfo->ScreenPosX, &this->WInfo->ScreenPosY);

}

void CRenderer::Destroy()
{
	if (Window != nullptr)
	{
		SDL_DestroyWindow(Window);
	}
	TTF_Quit();
	
}

void CRenderer::Resize(int w, int h)
{
	SDL_SetWindowSize(Window, w, h);
	ScreenWidth = w;
	ScreenHeight = h;
}

void CRenderer::OnWindowMove()
{
	SDL_GetWindowPosition(this->Window, &this->WInfo->ScreenPosX, &this->WInfo->ScreenPosY);
}


SDL_Window* CRenderer::GetWindow()
{
	return Window;
}

std::shared_ptr<WindowInfo> CRenderer::GetWindowInfo()
{
	return this->WInfo;
}
