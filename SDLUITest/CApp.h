#pragma once

#include "SDL.h"
#include "CRenderer.h"
#include "CEvent.h"
#include "COpengl.h"
#include <vector>
#include "CObject3D.h"

#include "CLayout.h"
//#include "CLabel.h"
//#include "CButton.h"
//#include "CObject2D.h"

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

	COpengl OpenGL;
	CRenderer Renderer;
	CEvent Event;

	int MouseX = 0;
	int MouseY = 0;
	bool MouseLock=false;

	CLayout* Layout;
	/*CLabel* label;
	CLabel* label2;
	CButton* butt;*/


	vector<CObject3D> Objects3D;
	vector<CLayout> Objects2D;

};

