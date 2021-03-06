#pragma once

#ifdef __EMSCRIPTEN__
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif // __EMSCRIPTEN__
#include "CLog.h"
#include <fstream>
#include <array>
#include <memory>

using namespace std;
class CEvent
{
public:
	CEvent();
	~CEvent();
	void PollEvents();
	void GetMouseMotion(int& Mx, int& My);
	bool GetIsRunning();
	SDL_MouseButtonEvent GetMouseData();
	array<bool, 322> GetKeyboardData();
	SDL_Event GetEvent();
	uint32_t GetRawKey() { return this->RawKey; };
	bool GetIsEditing() { return this->IsEditing; };

private:
	SDL_Event _Event;
	fstream _File;

	int _MouseX = 0;
	int _MouseY = 0;

	array<bool, 322> Keys;

	SDL_KeyboardEvent KeyboardEvent;
	SDL_MouseButtonEvent MouseEvent;
	uint32_t RawKey;
	bool IsEditing = false;
	bool _IsRunning = true;
};

extern std::unique_ptr<CEvent> Event;
