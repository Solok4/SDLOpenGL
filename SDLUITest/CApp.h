#pragma once

#ifndef CAPP
#define CAPP


#ifdef __EMSCRIPTEN__
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif // __EMSCRIPTEN__

#include <vector>
#include <array>
#include <thread>

#include "CRenderer.h"
#include "CEvent.h"
#include "COpengl.h"
#include "CObject3D.h"

#include "CLayoutManager.h"
#include "CModelManager.h"
#include "CStaticMeshComponent.h"
#include "CSceneManager.h"
#include "CKeyboardConf.h"
#include "CGameplayManager.h"


	bool Init();
	void Destroy();
	void Loop();
	void PollEvents();
	void PreLoop();
	void SetMouseLock(bool lock);
	void ResizeWindow(int w, int h);
	void SetFPSLock(int FPS);

	void KeyEvents(array<bool,322> keys);


	/*int MouseX;
	int MouseY;
	bool MouseLock;
	int WindowW;
	int WindowH;

	int FPSLock;

	uint32_t Start;
	uint32_t End;
	uint32_t FrameTime;*/

#endif // !CAPP
