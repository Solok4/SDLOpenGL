#pragma once


#ifdef __EMSCRIPTEN__
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#else
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#endif // __EMSCRIPTEN__

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2_ttf.lib")
#pragma comment(lib,"SDL2_image.lib")


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
	TTF_Font* Font;

};

