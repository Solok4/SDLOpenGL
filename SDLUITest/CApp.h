#pragma once

#ifdef __EMSCRIPTEN__
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif // __EMSCRIPTEN__

#include "CRenderer.h"
#include "CEvent.h"
#include "COpengl.h"
#include <vector>
#include <array>
#include <thread>
#include "CObject3D.h"

#include "CLayoutManager.h"
#include "CModelManager.h"
#include "CStaticMeshComponent.h"
#include "CSceneManager.h"
#include "CKeyboardConf.h"
#include "CGameplayManager.h"


class CApp
{
public:
	CApp();
	~CApp();

	bool Init();
	void Destroy();
	void Loop();
	void PollEvents();
	void PreLoop();
	void SetMouseLock(bool lock);
	void GetWindowSize(int &w, int &h) { w = this->WindowH; h = this->WindowH; };
	void ResizeWindow(int w, int h);
	void SetFPSLock(int FPS);
	uint32_t GetFrameTime() { return this->FrameTime; };
#ifdef __EMSCRIPTEN__
	void EmscriptenLoop();
#endif // __EMSCRIPTEN__

	void KeyEvents(array<bool,322> keys);

	COpengl OpenGL;
	CRenderer Renderer;
	CEvent Event;

	int MouseX;
	int MouseY;
	bool MouseLock;
	int WindowW;
	int WindowH;

	int FPSLock;

	uint32_t Start;
	uint32_t End;
	uint32_t FrameTime;

	std::unique_ptr<CLayoutManager> LayoutManager;
	std::unique_ptr<CModelManager> ModelManager;
	std::unique_ptr<CSceneManager> SceneManager;
	std::unique_ptr<CKeyboardConf> KeyboardConf;
	std::unique_ptr<CGameplayManager> GameplayManager;

};

