#include "pch.h"
#include "CLog.h"
#include "CRenderer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"



CRenderer::CRenderer()
{
}


CRenderer::~CRenderer()
{
}

void CRenderer::Init()
{
	Window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (Window == nullptr)
	{
		CLog::MyLog(1, "Failed to create window: " + std::string(SDL_GetError()));

	}
	if (TTF_Init() != 0)
	{
		CLog::MyLog(1, "Failed to initalize TTF: " + std::string(TTF_GetError()));
	}
	if (!IMG_Init(IMG_INIT_JPG))
	{
		CLog::MyLog(1, "Failed to initalize Image library: " + std::string(IMG_GetError()));
	}
}

void CRenderer::Destroy()
{
	//TTF_CloseFont(Font);
	if (Window != nullptr)
	{
		SDL_DestroyWindow(Window);
	}
	TTF_Quit();
	IMG_Quit();
	
}

void CRenderer::Resize(int w, int h)
{
	SDL_SetWindowSize(Window, w, h);
	ScreenWidth = w;
	ScreenHeight = h;
}

void CRenderer::OpenFont(const char* path)
{
	this->Font = TTF_OpenFont(path, 32);
	if (!this->Font)
	{
		CLog::MyLog(1, "Failed to load font " + std::string(path));
	}
}

TTF_Font* CRenderer::GetFont()
{
	return this->Font;
}

SDL_Window* CRenderer::GetWindow()
{
	return Window;
}
