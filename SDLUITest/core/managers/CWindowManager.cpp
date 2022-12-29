#include "CWindowManager.h"

std::unique_ptr<CWindowManager> WindowManager;

CWindowManager::CWindowManager()
{
	this->Window = nullptr;
}

CWindowManager::~CWindowManager()
{
	CLog::debug("WindowManagerDestructor");
}

void CWindowManager::Init()
{
	this->WInfo = std::make_shared<WindowInfo>();
	this->DemandWinfo = std::make_shared<WindowInfo>();
	this->WInfo->ScreenHeight = 720;
	this->WInfo->ScreenWidth = 1280;
	this->WInfo->ScreenAspectRatio = (float)1280 / 720;
	this->WInfo->WindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	this->WInfo->BeginingOfTheFrame = std::chrono::system_clock::now();
	this->WInfo->EndOfTheFrame = std::chrono::system_clock::now();
	this->WInfo->Delta = this->WInfo->EndOfTheFrame - this->WInfo->BeginingOfTheFrame;
	this->WInfo->FPSLock = 60;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		CLog::error("Failed to initalize SDL");
	}

	Window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->WInfo->ScreenWidth, this->WInfo->ScreenHeight, this->WInfo->WindowFlags);
	if (Window == nullptr)
	{
		CLog::error("Failed to create window: %s", SDL_GetError());
	}
	CLog::debug("Window created");
	if (TTF_Init() != 0)
	{
		CLog::error("Failed to initalize TTF: %s", TTF_GetError());
	}
	CLog::debug("TTF initalized");

	SDL_GetWindowPosition(Window, &this->WInfo->ScreenPosX, &this->WInfo->ScreenPosY);
}

void CWindowManager::Destroy()
{
	if (Window != nullptr)
	{
		SDL_DestroyWindow(Window);
	}
	TTF_Quit();
}

void CWindowManager::Resize(int w, int h)
{
	SDL_SetWindowSize(this->Window, w, h);
	this->WInfo->ScreenWidth = w;
	this->WInfo->ScreenHeight = h;
	this->WInfo->ScreenAspectRatio = (float)this->WInfo->ScreenWidth / this->WInfo->ScreenHeight;
}

void CWindowManager::ResizeDemand()
{
	SDL_SetWindowSize(this->Window, this->DemandWinfo->ScreenWidth, this->DemandWinfo->ScreenHeight);
	this->WInfo->ScreenWidth = this->DemandWinfo->ScreenWidth;
	this->WInfo->ScreenHeight = this->DemandWinfo->ScreenHeight;
	this->WInfo->ScreenAspectRatio = (float)this->WInfo->ScreenWidth / this->WInfo->ScreenHeight;
}

void CWindowManager::OnWindowMove()
{
	SDL_GetWindowPosition(this->Window, &this->WInfo->ScreenPosX, &this->WInfo->ScreenPosY);
}

SDL_Window* CWindowManager::GetWindow()
{
	return Window;
}

std::shared_ptr<WindowInfo> CWindowManager::GetWindowInfo()
{
	return this->WInfo;
}

void CWindowManager::SetFrameLock(int frames)
{
	this->WInfo->FPSLock = frames;
}