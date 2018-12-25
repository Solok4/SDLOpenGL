#pragma once

#include "SDL.h"
#include "CRenderer.h"
#include "CEvent.h"
#include "COpengl.h"
#include <vector>
#include <array>
#include "CObject3D.h"

#include "CLayoutManager.h"
#include "CModelManager.h"
#include "CStaticMeshComponent.h"
#include "CSceneManager.h"

//#include "CLayout.h"


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

	void KeyEvents(array<bool,322> keys);

	COpengl OpenGL;
	CRenderer Renderer;
	CEvent Event;

	int MouseX = 0;
	int MouseY = 0;
	bool MouseLock=false;

	std::auto_ptr<CLayoutManager> LayoutManager;
	std::auto_ptr<CModelManager> ModelManager;
	std::auto_ptr<CSceneManager> SceneManager;

};

