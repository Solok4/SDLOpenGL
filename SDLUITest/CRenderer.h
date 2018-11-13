#pragma once

#include "SDL.h"
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2_ttf.lib")


class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	void Init();
	void Destroy();
	void Resize(int w, int h);
	SDL_Window* GetWindow();


private:

	SDL_Window* Window;
	Uint32 Flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	int ScreenWidth = 800;
	int ScreenHeight = 600;

};

