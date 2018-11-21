#pragma once

#include "SDL.h"
#include "CLog.h"
#include <fstream>

using namespace std;
class CEvent
{
public:
	CEvent();
	~CEvent();
	void PollEvents();
	int GetMouseMotion(bool getX);
	bool GetIsRunning();
	SDL_MouseButtonEvent GetMouseData();
	SDL_Event GetEvent();


private:
	SDL_Event _Event;
	fstream _File;

	int _MouseX = 0;
	int _MouseY = 0;

	int KeyPress;
	int KeyRelease;
	SDL_MouseButtonEvent MouseEvent;

	bool _IsRunning = true;
	
};

