// SDLUITest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "CApp.h"
#define main main
#ifdef __EMSCRIPTEN__

#include <emscripten.h>

void Loop();
std::function<void()> Lp;
#endif


int main()
{
	CApp app;

	if (!app.Init())
	{
		app.Destroy();
	}
	app.Loop();
#ifdef __EMSCRIPTEN__
	Lp = [&]
	{
		app.Loop();
	};
	emscripten_set_main_loop(Loop, 0, true);
#endif

	app.Destroy();
	return 0;
}

#ifdef __EMSCRIPTEN__
void Loop()
{
	Lp();
}
#endif // __EMSCRIPTEN__


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
