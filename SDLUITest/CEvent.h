#pragma once

#ifdef __EMSCRIPTEN__
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif // __EMSCRIPTEN__
#include "CLog.h"
#include <fstream>
#include <array>


using namespace std;
class CEvent
{
public:
	CEvent();
	~CEvent();
	void PollEvents();
	void GetMouseMotion(int &Mx, int &My);
	bool GetIsRunning();
	SDL_MouseButtonEvent GetMouseData();
	array<bool,322> GetKeyboardData();
	SDL_Event GetEvent();


private:
	SDL_Event _Event;
	fstream _File;

	int _MouseX = 0;
	int _MouseY = 0;

	array<bool, 322> Keys;

	SDL_KeyboardEvent KeyboardEvent;
	SDL_MouseButtonEvent MouseEvent;

	bool _IsRunning = true;
	
};

