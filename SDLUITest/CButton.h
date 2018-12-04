#pragma once
#include "CObject2D.h"
#include "CLabel.h"
#include <SDL.h>
class CButton :
	public CObject2D
{
public:
	CButton();
	~CButton();

	void IsClicked(SDL_MouseButtonEvent MouseData);

	void AttachFunc(void(*fun)());

	virtual void OnHover();

	virtual void Prepare() override;

	CLabel* GetLabel();

	void (*Function)();

	bool Pressed = false;

	int LastMouseX = 0;
	int LastMouseY = 0;

	int ScreenWidth=0;
	int ScreenHeight=0;

	CLabel* Label;
};

