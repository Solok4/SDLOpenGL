#pragma once
#include "CImage.h"
#include "CLabel.h"
#include <SDL.h>
#include <functional>

extern "C"
{
#include "Lua/lua.h"
#include "Lua/lualib.h"
#include "Lua/lauxlib.h"
}

class CButton :
	public CImage
{
public:
	CButton()=delete;
	CButton(const char* name, /*CLayout* ref,*/ glm::vec2 position, glm::vec2 objsize) :CImage(name, /*ref,*/ position, objsize) { this->_ID = Object2DType::OBJECT2D_BUTTON; };
	~CButton();
	//Checks if button is clicked, is being hovered or not. Calls aprioprate functions.
	void IsClicked(SDL_MouseButtonEvent MouseData);
	//Attaches function that happens when the button is clicked.
	void AttachFunc(std::function<void()> func);
	//Function that is called when button is being hovered.
	virtual void OnHover();
	//Function that is called when button isn't being hovered.
	virtual void OnMiss();
	//Links layout and the button.
	void SetLabel(std::shared_ptr<CLabel> label);
	//Returns label object if exists
	std::shared_ptr<CLabel> GetLabel();
	//Function being called when the button is clicked.

protected:
	bool Pressed = false;
	int LastMouseX = 0;
	int LastMouseY = 0;
	int ScreenWidth=0;
	int ScreenHeight=0;
	std::shared_ptr<CLabel> Label;
	std::function<void()> Func;
};

