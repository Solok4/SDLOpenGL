#pragma once


#ifdef __EMSCRIPTEN__
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#else
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <memory>
#endif // __EMSCRIPTEN__

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2_ttf.lib")
#pragma comment(lib,"SDL2_image.lib")

struct WindowInfo
{
	int ScreenWidth;
	int ScreenHeight;
	Uint32 WindowFlags;

	Uint32 Delta;
	Uint32 BeginingOfTheFrame;
	Uint32 EndOfTheFrame;

	Uint32 FPSLock;
};


class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	void Init();
	void Destroy();
	void Resize(int w, int h);
	SDL_Window* GetWindow();
	std::shared_ptr<WindowInfo> GetWindowInfo();


private:

	SDL_Window* Window;
	Uint32 Flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	int ScreenWidth = 1280;
	int ScreenHeight = 720;
	TTF_Font* Font;
	std::shared_ptr<WindowInfo> WInfo;

};

extern std::unique_ptr<CRenderer> Renderer;

