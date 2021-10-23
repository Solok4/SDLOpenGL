#include "CButton.h"
#include <SDL.h>
#include "../../CLog.h"
#include "../renderer/COpengl.h"

CButton::~CButton()
{
}

void CButton::IsClicked(SDL_MouseButtonEvent MouseData)
{
	if (this->_IsActive)
	{
		if (this->_Position.x < MouseData.x && this->_Position.x + this->_Size.x > MouseData.x && this->_Position.y < MouseData.y && this->_Position.y + this->_Size.y > MouseData.y)
		{
			if (MouseData.button == SDL_BUTTON_LEFT && MouseData.type == SDL_MOUSEBUTTONDOWN)
			{
				this->IsHitLPM = true;
				this->IsLastHit = true;
				this->SetColor(vec3(1.0f));
				return;
			}
			else if (MouseData.button == SDL_BUTTON_LEFT && MouseData.type == SDL_MOUSEBUTTONUP)
			{
				if (this->IsHitLPM)
				{
					this->IsHitLPM = false;
					this->CallFunction(MouseButton::LEFTMOUSEBUTTON);
				}
			}
			else
			{
				if (!this->IsHitLPM)
				{
					this->OnHover();
				}
				else
				{
					this->IsHitLPM = !this->IsHitLPM;
				}
			}

			if (MouseData.button == SDL_BUTTON_RIGHT && MouseData.type == SDL_MOUSEBUTTONDOWN)
			{
				this->IsHitPPM = true;
				this->IsLastHit = true;
				this->SetColor(vec3(1.0f));
				return;
			}
			else if (MouseData.button == SDL_BUTTON_RIGHT && MouseData.type == SDL_MOUSEBUTTONUP)
			{
				if (this->IsHitPPM)
				{
					this->IsHitPPM = false;
					this->CallFunction(MouseButton::RIGTHTMOUSEBUTTON);
				}
			}
			else
			{
				if (!this->IsHitPPM)
				{
					this->OnHover();
				}
				else
				{
					this->IsHitPPM = !this->IsHitPPM;
				}
			}
		}
		else
		{
			if (MouseData.button == SDL_BUTTON_LEFT && MouseData.type == SDL_MOUSEBUTTONDOWN)
			{
				this->IsLastHit = false;
			}
			this->IsHitPPM = false;
			this->IsHitLPM = false;
			this->OnMiss();
		}
	}
	else
	{
		this->OnMiss();
	}
}

void CButton::AttachFunc(std::function<void()> func, MouseButton button)
{
	for (int i = 0; i < this->Funcs.size(); i++)
	{
		if (button == this->Funcs[i]->button)
		{
			this->Funcs.erase(this->Funcs.begin() + i);
			this->Funcs.push_back(std::make_shared<MouseFunc>(button, func));
			return;
		}
	}
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

void CButton::CallFunction(MouseButton button)
{
	for (auto a : this->Funcs)
	{
		if (a->button == button)
		{
			a->func();
		}
	}
}

void CButton::PostDraw()
{
	if (this->_IsVisible)
	{
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		this->Label->PreDraw();
		OpenGL->SetModelMatrix(this->Label->GetModelMatrix());
		OpenGL->SetColorMaskLayout(this->Label->GetColorMask());
		this->Label->Draw();
		this->Label->PostDraw();
	}
}