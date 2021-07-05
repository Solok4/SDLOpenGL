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
#include "CLog.h"

#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2_ttf.lib")
#pragma comment(lib,"SDL2_image.lib")

struct WindowInfo
{
	WindowInfo() :ScreenWidth(0), ScreenHeight(0), ScreenPosX(0), ScreenPosY(0), ScreenAspectRatio(0.0f), WindowFlags(0),
		Delta(0), BeginingOfTheFrame(std::chrono::system_clock::now()), EndOfTheFrame(std::chrono::system_clock::now()), FPSLock(30) {};
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

class CWindowManager
{
public:
	CWindowManager();
	~CWindowManager();

	//Initalizes window
	void Init();
	//Destroys Window
	void Destroy();
	//Change size of the window
	void Resize(int w, int h);
	void ResizeDemand();
	//Called on window move event
	void OnWindowMove();
	//Returns SDL_Window
	SDL_Window* GetWindow();
	//Returns window information struct
	std::shared_ptr<WindowInfo> GetWindowInfo();
	std::shared_ptr<WindowInfo> GetDemandWindowInfo() { return this->DemandWinfo; };

	void SetFrameLock(int frames);

private:

	SDL_Window* Window;
	std::shared_ptr<WindowInfo> WInfo;
	std::shared_ptr<WindowInfo> DemandWinfo;
};

extern std::unique_ptr<CWindowManager> Renderer;
