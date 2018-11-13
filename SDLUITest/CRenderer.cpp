#include "pch.h"
#include "CLog.h"
#include "CRenderer.h"
#include "SDL_ttf.h"



CRenderer::CRenderer()
{
}


CRenderer::~CRenderer()
{
	Destroy();
}

void CRenderer::Init()
{
	Window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (Window == nullptr)
	{
		CLog::MyLog(1, "Failed to create window");

	}
	if (TTF_Init() != 0)
	{
		CLog::MyLog(1, "Failed to initalize TTF");
	}
}

void CRenderer::Destroy()
{
	if (Window != nullptr)
	{
		SDL_DestroyWindow(Window);
	}
	
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
