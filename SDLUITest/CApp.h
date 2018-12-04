#pragma once

#include "SDL.h"
#include "CRenderer.h"
#include "CEvent.h"
#include "COpengl.h"
#include <vector>
#include <array>
#include "CObject3D.h"

#include "CLayout.h"


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
	void TempLayout();

	void KeyEvents(array<bool,322> keys);

	COpengl OpenGL;
	CRenderer Renderer;
	CEvent Event;

	int MouseX = 0;
	int MouseY = 0;
	bool MouseLock=false;

	CLayout* Layout;


	vector<CObject3D*> Objects3D;
	vector<CLayout*> Objects2D;

};

