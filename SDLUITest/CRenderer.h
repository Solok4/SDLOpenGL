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

#include <chrono>

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2_ttf.lib")
#pragma comment(lib,"SDL2_image.lib")

struct WindowInfo
{
	int ScreenWidth;
	int ScreenHeight;
	int ScreenPosX;
	int ScreenPosY;
	float ScreenAspectRatio;
	uint32_t WindowFlags;

	std::chrono::duration<double> Delta;
	std::chrono::time_point<std::chrono::system_clock> BeginingOfTheFrame;
	std::chrono::time_point<std::chrono::system_clock> EndOfTheFrame;

	uint32_t FPSLock;
};


class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	//Initalizes window
	void Init();
	//Destroys Window
	void Destroy();
	//Change size of the window
	void Resize(int w, int h);
	//Called on window move event
	void OnWindowMove();
	//Returns SDL_Window 
	SDL_Window* GetWindow();
	//Returns window information struct
	std::shared_ptr<WindowInfo> GetWindowInfo();

	void SetFrameLock(int frames);


private:

	SDL_Window* Window;
	Uint32 Flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	int ScreenWidth = 1280;
	int ScreenHeight = 720;
	std::shared_ptr<WindowInfo> WInfo;

};

extern std::unique_ptr<CRenderer> Renderer;

