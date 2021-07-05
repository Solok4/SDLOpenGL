// SDLUITest.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include "CApp.h"
#define main main
#ifdef __EMSCRIPTEN__

#include <emscripten.h>

#endif

int main(int argc, char** argv)
{
	if (!Init(argc, argv))
	{
		Destroy();
	}
	Loop();
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(Loop, Renderer->GetWindowInfo()->FPSLock, true);
#endif

	Destroy();
	return 0;
}