#pragma once

#include "SDL.h"
#include "CRenderer.h"
#include "CEvent.h"
#include "COpengl.h"
#include <vector>
#include "CObject2D.h"
#include "CObject3D.h"
#include "CLabel.h"
#include "CButton.h"

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
	void MapButtons();
	void SetMouseLock(bool lock);

	COpengl OpenGL;
	CRenderer Renderer;
	CEvent Event;

	int MouseX = 0;
	int MouseY = 0;
	bool MouseLock=false;

	CLabel* label;
	CLabel* label2;
	CButton* butt;


	vector<CObject3D> Objects3D;
	vector<CObject2D> Objects2D;

};

