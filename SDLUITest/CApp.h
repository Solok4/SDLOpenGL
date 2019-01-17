#pragma once

#ifdef __EMSCRIPTEN__
#include <SDL.h>
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
	uint32_t GetFrameTime() { return this->FrameTime; };
#ifdef __EMSCRIPTEN__
	void EmscriptenLoop();
#endif // __EMSCRIPTEN__

	void KeyEvents(array<bool,322> keys);

	COpengl OpenGL;
	CRenderer Renderer;
	CEvent Event;

	int MouseX = 0;
	int MouseY = 0;
	bool MouseLock=false;
	uint32_t FrameTime;
	int WindowW;
	int WindowH;

	uint32_t Start;
	uint32_t End;

	std::unique_ptr<CLayoutManager> LayoutManager;
	std::unique_ptr<CModelManager> ModelManager;
	std::unique_ptr<CSceneManager> SceneManager;

};

