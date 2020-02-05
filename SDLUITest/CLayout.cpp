#include "pch.h"
#include "CLayout.h"
#include <memory>
#include "COpengl.h"
#include "CEvent.h"



CLayout::CLayout()
{
	this->RefreshWindowData();
	this->UsedTestBox = nullptr;
}


CLayout::~CLayout()
{
	CLog::MyLog(LogType::Log, "LayoutDestructor %s",this->Name);
}

void CLayout::RefreshWindowData()
{
	this->WNDInfo = Renderer->GetWindowInfo();
}

void CLayout::PrepareToLoop()
{
	for (auto a : Objects2D)
	{
		a->Prepare();
	}
}

void CLayout::Draw()
{
	glDisable(GL_CULL_FACE);

	for (auto a : Objects2D)
	{
		a->PreDraw();
		OpenGL->SetModelMatrixLayout(a->GetModelMatrix());
		OpenGL->SetColorMaskLayout(a->GetColorMask());
		a->Draw();
		a->PostDraw();
	}
	glEnable(GL_CULL_FACE);
}

void CLayout::AddItem(int id, const char* name, glm::vec2 pos, glm::vec2 size)
{
	if (id == Object2DType::OBJECT2D_LABEL)	//Clabel
	{
		std::shared_ptr<CLabel> temp = std::make_shared<CLabel>(name,pos,size,this);
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_IMAGE)	//CImage
	{
		std::shared_ptr<CImage> temp = std::make_shared<CImage>(name, pos, size,this);
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_BUTTON)	//Button
	{
		char LabelName[64];
		std::shared_ptr<CButton> temp = std::make_shared<CButton>(name, pos, size, this);

#ifndef __EMSCRIPTEN__
		sprintf_s(LabelName, "%s%s", name, "_Label");
#else
		sprintf(LabelName, "%s%s", name, "_Label");
#endif
		std::shared_ptr<CLabel> ButtonLabel = std::make_shared<CLabel>(LabelName, glm::vec2(temp->GetSize().x / 2, temp->GetSize().y / 2),glm::vec2(1.f), this);
		ButtonLabel->BindParrentObject(temp);
		ButtonLabel->MoveObjectLayerUp();
		ButtonLabel->SetPadding(glm::vec2(10.f));
		ButtonLabel->SetAligment(Object2DAligment::CENTER);
		Objects2D.push_back(temp);
		CLayout::AddButtonToList(temp);

		temp->SetLabel(ButtonLabel);
		Objects2D.push_back(ButtonLabel);
	}
	else if (id == Object2DType::OBJECT2D_CONTAINER)	//CContainer
	{
		std::shared_ptr<CContainer> temp = std::make_shared<CContainer>(name, pos, size, this);
		Objects2D.push_back(temp);
	}
	else if (id == Object2DType::OBJECT2D_TEXTBOX)		//CTextBox
	{
		char LabelName[64];
		std::shared_ptr<CTextBox> temp = std::make_shared<CTextBox>(name, pos, size, this);
#ifndef __EMSCRIPTEN__
		sprintf_s(LabelName, "%s%s", name, "_Label");
#else
		sprintf(LabelName, "%s%s", name, "_Label");
#endif
		std::shared_ptr<CLabel> TextBoxLabel = std::make_shared<CLabel>(LabelName, glm::vec2(temp->GetSize().x / 2, temp->GetSize().y / 2), glm::vec2(1.f), this);
		TextBoxLabel->BindParrentObject(temp);
		TextBoxLabel->MoveObjectLayerUp();
		TextBoxLabel->SetPadding(glm::vec2(10.f));
		TextBoxLabel->SetAligment(Object2DAligment::CENTER);
		Objects2D.push_back(temp);
		CLayout::AddButtonToList(temp);

		temp->SetLabel(TextBoxLabel);
		Objects2D.push_back(TextBoxLabel);
	}
}

std::vector<std::shared_ptr<CObject2D>> CLayout::GetButtons()
{
	return this->Buttons;
}

void CLayout::AddButtonToList(std::shared_ptr<CObject2D> btn)
{
	auto temp = std::dynamic_pointer_cast<CButton>(btn);
	if (temp != nullptr)
	{
		this->Buttons.push_back(btn);
		return;
	}
	temp = std::dynamic_pointer_cast<CTextBox>(btn);
	if (temp != nullptr)
	{
		this->Buttons.push_back(btn);
		return;
	}
}

const char* CLayout::GetName()
{
	return this->Name;
}

void CLayout::SetName(const char* name)
{
	this->Name = name;
}

void CLayout::Tick(double delta)
{
	Event->GetMouseMotion(this->MousePosX, this->MousePosY);
	for (auto a : Objects2D)
	{
		a->Tick(delta);
	}
	auto buttons = this->GetButtons();
	for (auto b : buttons)
	{
		auto butt = std::dynamic_pointer_cast<CButton>(b);
		if (butt != nullptr)
		{
			butt->IsClicked(Event->GetMouseData());
		}
	}
	this->ProcessTextEditing();
}

void CLayout::ProcessTextEditing()
{
	if (this->UsedTestBox == nullptr)
	{
		return;
	}
	if (this->LayoutIsEditing)
	{

		bool bChanged = false;
		uint32_t SingleKey = Event->GetRawKey();
		std::string Text = this->UsedTestBox->GetValue();
		if (SingleKey == SDLK_BACKSPACE && Text.length() > 0)
		{
			Text.pop_back();
			bChanged = true;
		}
		else if (SingleKey == SDLK_c && SDL_GetModState() & KMOD_CTRL)
		{
			SDL_SetClipboardText(Text.c_str());
		}
		else if (SingleKey == SDLK_v && SDL_GetModState() & KMOD_CTRL)
		{
			Text = SDL_GetClipboardText();
		}

		if (Event->GetIsEditing())
		{
			Text.push_back(SingleKey);
			bChanged = true;
		}
		if (bChanged)
		{
			this->UsedTestBox->SetValue(Text);
		}
	}
}

void CLayout::SetUsedTextBox(CTextBox* tbox)
{
	if (tbox != nullptr)
	{
		SDL_StartTextInput();
		this->UsedTestBox = tbox;
		this->LayoutIsEditing = true;
	}
	else
	{
		SDL_StopTextInput();
		this->UsedTestBox = nullptr;
		this->LayoutIsEditing = false;
	}
}
