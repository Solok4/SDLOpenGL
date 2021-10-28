// SDLUITest.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "CApp.h"
#define main main

int main(int argc, char** argv)
{
	if (!Init(argc, argv))
	{
		Destroy();
	}
	Loop();
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(Loop, WindowManager->GetWindowInfo()->FPSLock, true);
#endif

	Destroy();
	return 0;
}