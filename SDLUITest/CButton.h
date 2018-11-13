#pragma once
#include "CObject2D.h"
#include <SDL.h>
class CButton :
	public CObject2D
{
public:
	CButton(unsigned int ID);
	~CButton();

	void IsClicked(SDL_MouseButtonEvent MouseData);
	void AttachFunc(void(*fun)());
	virtual void OnClick();

	virtual void Prepare() override;

	void (*Function)();

	int ScreenWidth=0;
	int ScreenHeight=0;

	uint32 start, current;

};

