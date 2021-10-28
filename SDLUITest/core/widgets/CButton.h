#pragma once
#include "CImage.h"
#include "CLabel.h"
#include <SDL.h>
#include <functional>

#include "../shared/Shared.h"

enum MouseButton
{
	LEFTMOUSEBUTTON = 0,
	RIGTHTMOUSEBUTTON,
	MIDDLEMOUSEBUTTON,
};

struct MouseFunc
{
	MouseFunc(MouseButton butt, std::function<void()> fun) :button(butt), func(fun) {};
	MouseButton button;
	std::function<void()> func;
};

class CButton :
	public CImage
{
public:
	CButton() = delete;
	CButton(const char* name, glm::vec2 position, glm::vec2 objsize, CLayout* ref) :CImage(name, position, objsize, ref)
	{
		this->_ID = Object2DType::OBJECT2D_BUTTON;
		this->Funcs.push_back(std::make_shared<MouseFunc>(MouseButton::LEFTMOUSEBUTTON, []() {}));
		this->Funcs.push_back(std::make_shared<MouseFunc>(MouseButton::RIGTHTMOUSEBUTTON, []() {}));
		this->Funcs.push_back(std::make_shared<MouseFunc>(MouseButton::MIDDLEMOUSEBUTTON, []() {}));

		char LabelName[64];
		StdLibWrapper::Ssprintf(LabelName, "%s%s", name, "_Label");
		this->Label = std::make_shared<CLabel>(LabelName, glm::vec2(0.0f), glm::vec2(1.f), this->LayoutRef);
		this->Label->BindParrentObject(this);
		this->Label->MoveObjectLayerUp();
		this->Label->SetPadding(glm::vec2(0.f));
		this->Label->SetAligment(Object2DAligment::CENTER);
		this->Label->Prepare();
	};
	~CButton();
	//Checks if button is clicked, is being hovered or not. Calls aprioprate functions.
	void IsClicked(SDL_MouseButtonEvent MouseData);
	//Attaches function that happens when the button is clicked.
	void AttachFunc(std::function<void()> func, MouseButton button);
	//Function that is called when button is being hovered.
	virtual void OnHover();
	//Function that is called when button isn't being hovered.
	virtual void OnMiss();
	//Links layout and the button.
	void SetLabel(std::shared_ptr<CLabel> label);
	//Returns label object if exists
	std::shared_ptr<CLabel> GetLabel();
	//Function being called when the button is clicked.
	void CallFunction(MouseButton button);

	//Cleans up things after drawing.
	virtual void PostDraw() override;

	bool GetIsLastHit() { return this->IsLastHit; };

protected:
	bool IsHitLPM = false;
	bool IsHitPPM = false;

	bool IsLastHit = false;
	int LastMouseX = 0;
	int LastMouseY = 0;
	int ScreenWidth = 0;
	int ScreenHeight = 0;
	std::shared_ptr<CLabel> Label;
	std::vector<std::shared_ptr<MouseFunc>> Funcs;
};
