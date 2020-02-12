#include "pch.h"
#include "CRenderer.h"


std::unique_ptr<CRenderer> Renderer;

CRenderer::CRenderer()
{
}


CRenderer::~CRenderer()
{
	CLog::MyLog(LogType::Debug, "RendererDestructor");
}

void CRenderer::Init()
{
	this->WInfo = std::make_shared<WindowInfo>();
	this->DemandWinfo = std::make_shared<WindowInfo>();
	this->WInfo->ScreenHeight = 720;
	this->WInfo->ScreenWidth = 1280;
	this->WInfo->ScreenAspectRatio = (float)1280 / 720;
	this->WInfo->WindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	this->WInfo->BeginingOfTheFrame = std::chrono::system_clock::now();
	this->WInfo->EndOfTheFrame = std::chrono::system_clock::now();
	this->WInfo->Delta = this->WInfo->EndOfTheFrame- this->WInfo->BeginingOfTheFrame;
	this->WInfo->FPSLock = 60;

	Window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->WInfo->ScreenWidth, this->WInfo->ScreenHeight, this->WInfo->WindowFlags);
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
	SDL_SetWindowSize(this->Window, w, h);
	this->WInfo->ScreenWidth = w;
	this->WInfo->ScreenHeight = h;
	this->WInfo->ScreenAspectRatio = (float)this->WInfo->ScreenWidth / this->WInfo->ScreenHeight;
}

void CRenderer::ResizeDemand()
{
	SDL_SetWindowSize(this->Window, this->DemandWinfo->ScreenWidth, this->DemandWinfo->ScreenHeight);
	this->WInfo->ScreenWidth = this->DemandWinfo->ScreenWidth;
	this->WInfo->ScreenHeight = this->DemandWinfo->ScreenHeight;
	this->WInfo->ScreenAspectRatio = (float)this->WInfo->ScreenWidth / this->WInfo->ScreenHeight;
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

void CRenderer::SetFrameLock(int frames)
{
	this->WInfo->FPSLock = frames;
}
