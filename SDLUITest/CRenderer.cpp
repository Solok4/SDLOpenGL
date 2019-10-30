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
	this->WInfo->WindowFlags = this->Flags;
	this->WInfo->BeginingOfTheFrame = 0;
	this->WInfo->EndOfTheFrame = 0;
	this->WInfo->Delta = 0;
	this->WInfo->FPSLock = 60;

	Window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, Flags);
	if (Window == nullptr)
	{
		CLog::MyLog(LogType::Error, "Failed to create window: %s",SDL_GetError());

	}
	if (TTF_Init() != 0)
	{
		CLog::MyLog(LogType::Error, "Failed to initalize TTF: %s",TTF_GetError());
	}

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


SDL_Window* CRenderer::GetWindow()
{
	return Window;
}

std::shared_ptr<WindowInfo> CRenderer::GetWindowInfo()
{
	return this->WInfo;
}
