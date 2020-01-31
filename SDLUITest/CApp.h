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
#include "CInitialSetup.h"


	bool Init(int argc, char** argv);
	void Destroy();
	void Loop();
	void PollEvents();
	void PreLoop();
	//void SetMouseLock(bool lock);
	//void ResizeWindow(int w, int h);
	//void SetFPSLock(int FPS);

#endif // !CAPP
