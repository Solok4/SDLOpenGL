#include "pch.h"
#include "CButton.h"
#include <SDL.h>
#include "CLog.h"


CButton::CButton() : CObject2D()
{
	this->_Size = vec2(1.0f);
	this->_Position = vec3(0.0f);
	this->_Rotation = vec3(0.f);
	this->ColorMask = vec4(1.f);
}


CButton::~CButton()
{
}

void CButton::IsClicked(SDL_MouseButtonEvent MouseData)
{

	//if (this->_Position.x-(this->_Size.x/2)<MouseData.x &&this->_Position.x + (this->_Size.x / 2) >MouseData.x && this->_Position.y - (this->_Size.y / 2) <MouseData.y && this->_Position.y + (this->_Size.y / 2) >MouseData.y)
	if (this->_Position.x < MouseData.x && this->_Position.x + this->_Size.x > MouseData.x && this->_Position.y < MouseData.y && this->_Position.y + this->_Size.y > MouseData.y)
	{
		if (this->_IsActive)
		{

			if (MouseData.button == SDL_BUTTON_LEFT && MouseData.type == SDL_MOUSEBUTTONDOWN)
			{
				if (!this->Pressed)
				{
					this->Pressed = true;
					this->SetColor(vec3(1.0f));
				}
			}
			else if (MouseData.button == SDL_BUTTON_LEFT && MouseData.type == SDL_MOUSEBUTTONUP)
			{
				if (this->Pressed)
				{
					this->Pressed = false;
					this->Func();
					this->OnHover();
				}
			}
			else
			{
				if (!this->Pressed)
					this->OnHover();
			}
		}
	}
	else
	{
		this->Pressed = false;
		this->OnMiss();
	}
}

void CButton::AttachFunc(std::function<void()> func)
{
	this->Func = func;
}

void CButton::OnHover()
{
	this->SetColor(vec3(0.7f));
}

void CButton::OnMiss()
{
	this->SetColor(vec3(0.3f));
}


void CButton::SetLabel(std::shared_ptr<CLabel> label)
{
	if (label->GetID() == Object2DType::OBJECT2D_LABEL)
	{
		this->Label = label;
	}
}


std::shared_ptr<CLabel> CButton::GetLabel()
{
	return this->Label;
}
